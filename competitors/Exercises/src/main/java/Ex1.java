import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Ex1 {

    public volatile static int X = 0, Y = 0;

    public static void ex2() throws InterruptedException {
        ReentrantReadWriteLock lockX = new ReentrantReadWriteLock(true), lockY = new ReentrantReadWriteLock(true);
        var thread2 = new Thread(() -> {
            lockX.writeLock().lock();
            var localX = X; // read
            X = localX+3; // write
            lockX.writeLock().unlock(); //commit
        });
        var thread1 = new Thread(() -> {
            lockX.readLock().lock();
            var localX = X; // read
            lockY.readLock().lock();
            var localY = Y;
            lockY.readLock().unlock();
            lockX.readLock().unlock();
        });
        thread1.start(); thread2.start();
        thread1.join(); thread2.join();
        System.out.println("X = "+X+", Y="+Y);
    }

    public static void ex1() throws InterruptedException {
        ReentrantReadWriteLock lockX = new ReentrantReadWriteLock(true), lockY = new ReentrantReadWriteLock(true);
        var thread1 = new Thread(() -> {
            lockX.writeLock().lock();
            var tmpX = X;
            X = tmpX-2; // Write operation

            // Abort! re-setting the value back
            X = tmpX;
            lockX.writeLock().unlock();
        });
        var thread2 = new Thread(() -> {
            lockX.readLock().lock();
            var localX = X; // read
            lockY.writeLock().lock();
            var localY = Y;
            Y = localY+2; // write
            // commit!
            lockY.writeLock().unlock();
            lockX.readLock().unlock();
        });
        thread1.start(); thread2.start();
        thread1.join(); thread2.join();
        System.out.println("X = "+X+", Y="+Y);
    }

    public static void main(String args[]) throws InterruptedException {
     ex1();
    }

}
