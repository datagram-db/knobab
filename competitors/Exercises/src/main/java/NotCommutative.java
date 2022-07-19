import javax.management.Notification;

public class NotCommutative {

    private static volatile double countMe = 1.0;

    public static synchronized void doOperation(double value, boolean Mult_AddOtherwise) {
        if ((value == 5.0) && (countMe != 3.0))
        try { NotCommutative.class.wait(); } catch (Exception ignored) {}
        countMe = (Mult_AddOtherwise ? (countMe * value) : (countMe + value));
        NotCommutative.class.notifyAll();
    }

    public static void main(String args[]) throws InterruptedException {

        for (int i = 0; i<1000000; i++) {
            countMe = 1.0;
            Thread t1 = new Thread(() -> doOperation(3.0, true));
            Thread t2 = new Thread(() -> doOperation(5.0, false));
            if (i % 2 == 0) {
                t1.start(); t2.start();
            } else {
                t2.start(); t1.start();
            }
            t1.join(); t2.join();
            System.out.println(i);
        }

        System.out.println(countMe);
    }

}
