import java.util.HashSet;
import java.util.Set;

public class CorrectTheProgram {

    private static volatile double countMe = 1.0;

    public static void doOperation(double value, boolean Mult_AddOtherwise) {
        countMe = (Mult_AddOtherwise ? (countMe * value) : (countMe + value));
    }

    public static void main(String args[]) throws InterruptedException {
        Set<String> outcomes = new HashSet<>();
        for (int i = 0; i<500000; i++) {
            countMe = 1.0;
            Thread t1 = new Thread(() -> {
                doOperation(3.0, true);
            }, "Multiplier");
            Thread t2 = new Thread(() -> {
                doOperation(5.0, false);
            }, "Adder");
            t2.start(); t1.start();
            t1.join(); t2.join();
            outcomes.add(String.valueOf(countMe));
        }
        outcomes.forEach(System.out::println);
    }

}
