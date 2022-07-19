public class Test {
    private static volatile double countMe = 1.0;
    public static synchronized void doOperation(double value ,
                                                boolean Mult_AddOtherwise){
        if (value > countMe)
            try { Test.class.wait(); } catch (Exception ignored) {}
        countMe = (Mult_AddOtherwise ? (countMe * value) : (countMe + value));
        Test.class.notifyAll ();
    }
    public static void main(String args []) throws InterruptedException {
        countMe = 1.0;
        Thread t1 = new Thread (() -> doOperation (3.0, true));
        Thread t2 = new Thread (() -> doOperation (5.0, false));
        t2.start (); t1.start ();
        t1.join(); t2.join();
        System.out.println(countMe);
    }
}