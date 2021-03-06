// 使用 1 个 mutex 时造成死锁的情况：
// 1.1. 加锁之后忘记解锁
// 1.2. return 之前没有进行解锁
// 2.1. 重复的加锁
// 2.2. A()函数加锁, A()函数调用B()函数, B()函数中也进行加锁
// 3. 两个线程分别拿着两个共享资源的锁, 但都想访问对方的拿着的共享资源

// 如何避免死锁？
// 1. 避免多次锁定，多检查
// 2. 对共享资源访问完毕之后, 一定要解锁, 或者在加锁时使用trylock
// 3. 如果程序中有多把锁, 可以控制对锁的访问顺序, 
//    另外也可以在对其他互斥锁做加锁操作之前,先释放当前线程拥有的互斥锁
// 4. 项目程序中可以引用一些专门用于死锁检查的模块
