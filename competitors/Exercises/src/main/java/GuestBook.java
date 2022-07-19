import java.util.ArrayList;
import java.util.List;

public class GuestBook {

    private List<String> concatenation;

    public GuestBook() {
        concatenation = new ArrayList<>(2);
    }

    public synchronized void signBook(String user) {
        if (user.endsWith("son")) try { wait(); } catch (Exception e) {}
        concatenation.add("Greetings from " + user+" !");
        notify();
    }

    public static void main(String[] args) throws InterruptedException {
        var book = new GuestBook();
        String localStars = "Rowan Atkinson; Eric Burdon";
        Thread t[] = new Thread[2];
        int i = 0;
        for (var star : localStars.split(";"))
            t[i++] = new Thread(() -> book.signBook(star));
        for (var thread : t) thread.start();
        for (var thread : t) thread.join();
        System.out.println(String.join(" and ", book.concatenation));
    }

}