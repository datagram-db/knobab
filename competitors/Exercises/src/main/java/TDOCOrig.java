import java.util.*;

public class TDOCOrig {

    public volatile static String giftList;

    public static void putGift(String giftId) {
        giftList += (giftId+"\n");
    }

    public static class DonorThread implements Runnable {

        private final int i;
        private final String[] giftArray;

        public DonorThread(int i, String[] giftArray) {
            this.i = i;
            this.giftArray = giftArray;
        }

        @Override
        public void run() {
            putGift(giftArray[i]);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        String gifts = "A Partridge in a Pear Tree,2 Turtle Doves,3 French Hens";
        String[] giftArray = gifts.split(",");
        Set<String> setOfPossibleOutputs = new HashSet<>();
        for (int j = 0; j<500000; j++ ){
            List<Thread> givers = new ArrayList<>(giftArray.length);
            giftList = "On a random day of Christmas\nmy true love sent to me:\n";
            for (int i = 0; i<giftArray.length; i++) {
                givers.add(new Thread(new DonorThread(i, giftArray), "Donor"+i));
            }
            Collections.shuffle(givers);
            givers.forEach(Thread::start);
            setOfPossibleOutputs.add(giftList);
            for (Thread t:  givers) t.join();
        }
        setOfPossibleOutputs.forEach(System.out::println);
    }
}
