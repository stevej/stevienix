//
// task.c - Implements the functionality needed to multitask.
//          Written for JamesM's kernel development tutorials.
//

#include "task.h"
#include "io.h"
#include "paging.h"
#include "printk.h"
#include "screen.h"

// The currently running task.
volatile task_t *current_task;

// The start of the task linked list.
volatile task_t *ready_queue;

// Some externs are needed to access members in paging.c...
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t *, int, int);
extern u32 initial_esp;
extern u32 read_eip();

// The next available process ID.
u32 next_pid = 1;

void initialise_tasking() {
  // Rather important stuff happening, no interrupts please!
  __asm__ volatile("cli");

  // Relocate the stack so we know where it is.
  move_stack((void *)0xE0000000, 0x2000);

  // Initialise the first task (kernel task)
  current_task = ready_queue = (task_t *)kmalloc(sizeof(*ready_queue));
  current_task->id = next_pid++;
  current_task->esp = current_task->ebp = 0;
  current_task->eip = 0;
  current_task->page_directory = current_directory;
  current_task->next = 0;
  current_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);

  // Reenable interrupts.
  __asm__ volatile("sti");
}

int queue_count(volatile task_t *queue) {
  int size = 1;
  while (queue->next) {
    size++;
    queue = queue->next;
  }

  return size;
}

void queue_watcher() {
  printk("[queue_watcher] %d\n", queue_count(ready_queue));
}

void move_stack(void *new_stack_start, u32 size) {
  u32 i;
  // Allocate some space for the new stack.
  for (i = (u32)new_stack_start; i >= ((u32)new_stack_start - size);
       i -= 0x1000) {
    // General-purpose stack is in user-mode.
    alloc_frame(get_page(i, 1, current_directory), 0 /* User mode */,
                1 /* Is writable */);
  }

  // Flush the TLB by reading and writing the page directory address again.
  u32 pd_addr;
  __asm__ volatile("mov %%cr3, %0" : "=r"(pd_addr));
  __asm__ volatile("mov %0, %%cr3" : : "r"(pd_addr));

  // Old ESP and EBP, read from registers.
  u32 old_stack_pointer;
  __asm__ volatile("mov %%esp, %0" : "=r"(old_stack_pointer));
  u32 old_base_pointer;
  __asm__ volatile("mov %%ebp, %0" : "=r"(old_base_pointer));

  // Offset to add to old stack addresses to get a new stack address.
  u32 offset = (u32)new_stack_start - initial_esp;

  // New ESP and EBP.
  u32 new_stack_pointer = old_stack_pointer + offset;
  u32 new_base_pointer = old_base_pointer + offset;

  // Copy the stack.
  memcpy((void *)new_stack_pointer, (void *)old_stack_pointer,
         initial_esp - old_stack_pointer);

  // Backtrace through the original stack, copying new values into
  // the new stack.
  for (i = (u32)new_stack_start; i > (u32)new_stack_start - size; i -= 4) {
    u32 tmp = *(u32 *)i;
    // If the value of tmp is inside the range of the old stack, assume it is a
    // base pointer and remap it. This will unfortunately remap ANY value in
    // this range, whether they are base pointers or not.
    if ((old_stack_pointer < tmp) && (tmp < initial_esp)) {
      tmp = tmp + offset;
      u32 *tmp2 = (u32 *)i;
      *tmp2 = tmp;
    }
  }

  // Change stacks.
  __asm__ volatile("mov %0, %%esp" : : "r"(new_stack_pointer));
  __asm__ volatile("mov %0, %%ebp" : : "r"(new_base_pointer));
}

void switch_task() {
  // If we haven't initialised tasking yet, just return.
  if (!current_task) {
    // screen_write(""); // bafflingly, this seems to stabilize task switching
    return;
  }

  // Read esp, ebp now for saving later on.
  u32 esp, ebp, eip;
  __asm__ volatile("mov %%esp, %0" : "=r"(esp));
  __asm__ volatile("mov %%ebp, %0" : "=r"(ebp));

  // Read the instruction pointer. We do some cunning logic here:
  // One of two things could have happened when this function exits -
  //   (a) We called the function and it returned the EIP as requested.
  //   (b) We have just switched tasks, and because the saved EIP is essentially
  //       the instruction after read_eip(), it will seem as if read_eip has
  //       just returned.
  // In the second case we need to return immediately. To detect it we put a
  // dummy value in EAX further down at the end of this function. As C returns
  // values in EAX, it will look like the return value is this dummy value!
  // (0x12345).
  eip = read_eip();
  /*screen_write("task.c: new eip:");
  screen_write_hex(eip);
  screen_write("\n");*/

  // Have we just switched tasks?
  if (eip == 0x12345) {
    // printk("task.c: switched tasks to %x\n", current_task->id);
    return;
  }

  // No, we didn't switch tasks. Let's save some register values and switch.
  current_task->eip = eip;
  current_task->esp = esp;
  current_task->ebp = ebp;

  // Get the next task to run.
  current_task = current_task->next;
  // If we fell off the end of the linked list start again at the beginning.
  if (!current_task)
    current_task = ready_queue;

  eip = current_task->eip;
  esp = current_task->esp;
  ebp = current_task->ebp;

  // Make sure the memory manager knows we've changed page directory.
  current_directory = current_task->page_directory;

  // Change our kernel stack over.
  //  set_kernel_stack(current_task->kernel_stack + KERNEL_STACK_SIZE);

  // Here we:
  // * Stop interrupts so we don't get interrupted.
  // * Temporarily puts the new EIP location in ECX.
  // * Loads the stack and base pointers from the new task struct.
  // * Changes page directory to the physical address (physicalAddr) of the new
  // directory.
  // * Puts a dummy value (0x12345) in EAX so that above we can recognise that
  // we've just
  //   switched task.
  // * Restarts interrupts. The STI instruction has a delay - it doesn't take
  // effect until after
  //   the next instruction.
  // * Jumps to the location in ECX (remember we put the new EIP in there).
  __asm__ volatile("         \
      cli;                 \
      mov %0, %%ecx;       \
      mov %1, %%esp;       \
      mov %2, %%ebp;       \
      mov %3, %%cr3;       \
      mov $0x12345, %%eax; \
      sti;                 \
      jmp *%%ecx           "
                   :
                   : "r"(eip), "r"(esp), "r"(ebp),
                     "r"(current_directory->physicalAddr));
}

u32 fork() {
  // We are modifying kernel structures, and so MUST NOT be interrupted.
  __asm__ volatile("cli");

  // Take a pointer to this process' task struct for later reference.
  task_t *parent_task = (task_t *)current_task;

  // Clone the address space.
  page_directory_t *directory = clone_directory(current_directory);

  // Create a new process.
  task_t *new_task = (task_t *)kmalloc(sizeof(task_t));

  new_task->id = next_pid++;
  new_task->esp = new_task->ebp = 0;
  new_task->eip = 0;
  new_task->page_directory = directory;
  // new_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);
  current_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);
  new_task->next = 0;

  // Add it to the end of the ready queue.
  task_t *tmp_task = (task_t *)ready_queue;
  while (tmp_task->next) {
    // screen_write("traversing ready_queue\n");
    tmp_task = tmp_task->next;
  }
  /*
  screen_write("adding task to ready_queue id:");
  screen_write_hex(new_task->id);
  screen_write(" eip:");
  screen_write_hex(new_task->eip);
  screen_write("\n"); */
  tmp_task->next = new_task;

  // This will be the entry point for the new process.
  u32 eip = read_eip();

  u32 rv = 0;
  // We could be the parent or the child here - check.
  if (current_task == parent_task) {
    // screen_write("We are the parent, so set up the esp/ebp/eip for our
    // child.\n");
    u32 esp;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp));
    u32 ebp;
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp));
    new_task->esp = esp;
    new_task->ebp = ebp;
    new_task->eip = eip;

    rv = new_task->id;
  }
  __asm__ volatile("sti");

  return rv;
}

// TODO(stevej): this does not quite work yet
void sys_exit(u32 errno) {
  // remove current_task from the ready_queue
  __asm__ volatile("cli");

  task_t *tmp_task = (task_t *)ready_queue;
  screen_write("current_task is exiting: ");
  screen_write_hex(current_task->id);
  screen_write("with errno: ");
  screen_write_dec(errno);
  screen_write("\n");

  do {
    screen_write("task.c: exiting task: ");
    screen_write_hex(current_task->id);
    screen_write("\n");
    if (current_task->next) {
      tmp_task->next = current_task->next; // then put it back in the queue
    } else {
      tmp_task->next = 0;
    }
  } while (tmp_task->next && tmp_task == current_task);

  __asm__ volatile("sti");
}

u32 getpid() { return current_task->id; }

void switch_to_user_mode() {
  // Set up our kernel stack.
  // set_kernel_stack(current_task->kernel_stack + KERNEL_STACK_SIZE);

  // Set up a stack structure for switching to user mode.
  __asm__ volatile("  \
      cli; \
      mov $0x23, %ax; \
      mov %ax, %ds; \
      mov %ax, %es; \
      mov %ax, %fs; \
      mov %ax, %gs; \
                    \
                    \
      mov %esp, %eax; \
      pushl $0x23; \
      pushl %esp; \
      pushf; \
      orl $0x200, (%esp); \
      pushl $0x1B; \
      push $1f; \
      iret; \
    1: \
    ");

  /*
  asm volatile("          \
              cli;            \
              mov $0x23, %eax;\
              mov %eax, %ds;  \
              mov %eax, %es;  \
              mov %eax, %fs;  \
              mov %eax, %gs;  \
                              \
              pushl $0x23;    \
                              \
              mov 0x1fffffff, %eax; \
              pushl %eax;     \
                              \
              pushf;          \
              orl $0x200,(%esp); \
                              \
              pushl $0x1B;    \
                              \
              push $1f;       \
                              \
              iret;   \
              1:      \
              ");*/

  /*
  // Set up a stack structure for switching to user mode.
  asm volatile("  \
    cli; \
    mov $0x23, %ax; \
    mov %ax, %ds; \
    mov %ax, %es; \
    mov %ax, %fs; \
    mov %ax, %gs; \
                  \
     \
    mov %esp, %eax; \
    pushl $0x23; \
    pushl %esp; \
    pushf; \
    orl $0x200,(%esp); \
    pushl $0x1B; \
    push $1f; \
    iret; \
  1: \
  ");*/
}
