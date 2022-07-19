import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Landfall {
    public enum Type {
        Island, Harbour, Sail
    }

    Type shipCurrentLocation;
    Long vehiclesMaxOccupancy;
    Map<Type, Long> vehiclesCount;

    Lock monitor;
    Condition ferryWaitFull;
    Map<Type, Condition> vehiclesWait;
    long countOK;

    public Landfall(long vehiclesMaxOccupancy) {
        monitor = new ReentrantLock(true);
        this.vehiclesMaxOccupancy = vehiclesMaxOccupancy;
        vehiclesCount = new HashMap<>(3);
        vehiclesWait = new HashMap<>(3);
        for (var x : Type.values()) {
            vehiclesCount.put(x, 0L);
            vehiclesWait.put(x, monitor.newCondition());
        }
        ferryWaitFull = monitor.newCondition();
        shipCurrentLocation = Type.Sail;
        countOK = 0;
    }

    public void onboarding_offboarding(Type pos) throws InterruptedException {
        if (pos == Type.Sail) throw new RuntimeException("ERROR: unexpected value");
        monitor.lock();
        try {
            if (shipCurrentLocation == pos) {
                // If the ship and the vehichle are at the same location

                while (vehiclesCount.get(Type.Sail).equals(vehiclesMaxOccupancy)) {
                    // If the ship reached the maximum occupancy
                    vehiclesCount.computeIfPresent(pos, (type, oldValue) -> oldValue + 1);
                    System.out.println(pos+" #veh = "+vehiclesCount.get(pos)+" L1");
                    vehiclesWait.get(pos).await();
                    vehiclesCount.computeIfPresent(pos, (type, oldValue) -> oldValue - 1);
                }

                // If the ship has some space left to ship cars
                vehiclesCount.computeIfPresent(Type.Sail, (type, oldValue) -> oldValue + 1);
                if (vehiclesCount.get(Type.Sail).equals(vehiclesMaxOccupancy))
                    ferryWaitFull.signal();
                vehiclesWait.get(Type.Sail).await();
                vehiclesCount.computeIfPresent(Type.Sail, (type, oldValue) -> oldValue - 1);
            } else {
                // otherwise, the car shall wait
                do {
                    vehiclesCount.computeIfPresent(pos, (type, oldValue) -> oldValue + 1);
                    System.out.println(pos+" #veh = "+vehiclesCount.get(pos)+" L2");
                    vehiclesWait.get(pos).await();
                    vehiclesCount.computeIfPresent(pos, (type, oldValue) -> oldValue - 1);
                } while (vehiclesCount.get(Type.Sail).equals(vehiclesMaxOccupancy));

                // If the ship has some space left to ship cars
                vehiclesCount.computeIfPresent(Type.Sail, (type, oldValue) -> oldValue + 1);
                if (vehiclesCount.get(Type.Sail).equals(vehiclesMaxOccupancy))
                    ferryWaitFull.signal();
                vehiclesWait.get(Type.Sail).await();
                vehiclesCount.computeIfPresent(Type.Sail, (type, oldValue) -> oldValue - 1);
            }
            countOK++;
        } finally {
            monitor.unlock();
        }
    }

    public void docking(Type pos, boolean lastTrip) throws InterruptedException {
        if (pos == Type.Sail) throw new RuntimeException("ERROR: unexpected value");
        monitor.lock();
        shipCurrentLocation = pos;
        try {
            // off-boarding
            Long N = Math.min(vehiclesMaxOccupancy, vehiclesCount.get(pos));
            for (int i = 0; i<vehiclesMaxOccupancy; i++) {
                vehiclesWait.get(Type.Sail).signal();
            }
            System.out.println(N+" vehicles to ship");
            for (int i = 0; i<N.intValue(); i++) {
                vehiclesWait.get(pos).signal();
            }
            if (!lastTrip)
                ferryWaitFull.await();
            else
                System.out.println("LAST TRIP");
        } finally {
            monitor.unlock();
        }
    }

    public void sail() {
        monitor.lock();
        try {
            shipCurrentLocation = Type.Sail;
        } finally {
            monitor.unlock();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        final int N = 4;
        final int maxOcc = 3;
        final int M = N * maxOcc * 2;
        Thread[] t = new Thread[M];
        Landfall monitor = new Landfall(maxOcc);
        for (int i = 0; i < M; i++) {
            int finalI = i;
            t[i] = new Thread(() -> {
                try {
                    var source = (((finalI/maxOcc) % 2) == 0) ? Type.Island : Type.Harbour;
                    var destination = (((finalI/maxOcc) % 2) == 1) ? Type.Island : Type.Harbour;
                    monitor.onboarding_offboarding(source);
                    System.out.println("Vehicle " + finalI + " reached destination " + destination.name());
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }, "Veh"+finalI);
        }
        Thread ferry = new Thread(() -> {
            try {
                for (int i = 0; i<N+1; i++) {
//                    System.out.println("["+i+"]");
//                    System.out.println("["+i+"] Reaching the harbour...");
                    monitor.docking(Type.Harbour, i == N);
//                    System.out.println("["+i+"] Sailing to the island...");
                    monitor.sail();
                    if (i != N) {
//                        System.out.println("["+i+"] Reaching the island...");
                        monitor.docking(Type.Island, false);
//                        System.out.println("["+i+"] Sailing back to the harbour...");
                        monitor.sail();
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }, "Ferry");
        ferry.start();
        for (int i = 0; i < M; i++) t[i].start();
        for (int i = 0; i < M; i++) t[i].join();
        ferry.join();
        System.out.println("Ferry Join...");
    }
}

