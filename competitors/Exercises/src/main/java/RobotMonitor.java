import java.util.*;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class RobotMonitor {

    public static class Pair<K, V> {
        public final K key;
        public final V value;
        public Pair(K key, V value) {
            this.key = key;
            this.value = value;
        }

        @Override
        public String toString() {
            return "(" +key +
                    "," + value +
                    ')';
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Pair<?, ?> pair = (Pair<?, ?>) o;
            return Objects.equals(key, pair.key) && Objects.equals(value, pair.value);
        }

        @Override
        public int hashCode() {
            return Objects.hash(key, value);
        }
    }

    public static double distance(Pair<Integer, Integer> x, Pair<Integer, Integer> y) {
         var xSq = x.key-y.key;
         var ySq = x.value-y.value;
         return xSq*xSq+ySq*ySq;
    }

    private Lock monitor;
    private Map<String, Pair<Integer,Integer>> moveRobotTo;
    private List<Pair<Integer,Integer>> robotPosition;
    private List<Pair<Integer, Integer>> positionsToGo;
    private Condition waitPosition;
    private int waiting_robots;

    public RobotMonitor(List<Pair<Integer,Integer>> robotInitialPosition) {
        monitor = new ReentrantLock(true);
        this.robotPosition = robotInitialPosition;
        moveRobotTo = new HashMap<>();
        positionsToGo = new LinkedList<>();
        waitPosition = monitor.newCondition();
        waiting_robots = 0;
    }

    public void moveRobotTo(int X, int Y) {
        monitor.lock();
        try {
            var dis = Thread.currentThread().getName();
            System.out.println(dis+" starting to request position: ("+X+","+Y+")");
            moveRobotTo.put(dis, new Pair<>(X, Y));
        } finally {
            monitor.unlock();
        }
    }

    public boolean setOKToMove() {
        boolean hasValue = true;
        monitor.lock();
        try {
            var dis = Thread.currentThread().getName();
            if (moveRobotTo.containsKey(dis)) {
                var pos = moveRobotTo.remove(dis);
                System.out.println(dis+" sending position: "+pos);
                positionsToGo.add(pos);
                if (waiting_robots>0) waitPosition.signalAll();
            } else {
                hasValue = false;
            }
        } finally {
            monitor.unlock();
        }
        return hasValue;
    }

    public Pair<Integer, Integer> handleRequest(int robotId) {
        Pair<Integer, Integer> candidate = null;
        monitor.lock();
        try {
            var currPos = robotPosition.get(robotId);
            var dis = Thread.currentThread().getName();
            if (positionsToGo.isEmpty())  {
                try {
                    waiting_robots++;
                    waitPosition.await();
                    waiting_robots--;
                } catch (InterruptedException e) { }
            }
            double minDistance = Double.MAX_VALUE;
            for (var position : positionsToGo) {
                double cmp = distance(position, currPos);
                if (cmp < minDistance) {
                    minDistance = cmp;
                    candidate = position;
                }
            }
            positionsToGo.remove(candidate);
            robotPosition.set(robotId, candidate);
        } finally {
            monitor.unlock();
        }
        return candidate;
    }

    public static Thread generateRobot(final RobotMonitor r, int i) {
        return new Thread(() -> {
            Random rnd = new Random();
            int j = 0;
            while (j<5) {
                System.out.println(i+" MOVING TO: " + r.handleRequest(i));
                j++;
            }
        });
    }

    public static Thread generateClient(final RobotMonitor r, int i) {
        return new Thread(() -> {
            Random rnd = new Random();
            int j = 0;
            while (true) {
                int x = rnd.nextInt(0, 10), y = rnd.nextInt(0, 10);
                System.out.println(i+" set robot moving to ("+x+","+y+")");
                r.moveRobotTo(x, y);
                // some operations
                r.setOKToMove();
                j++;
            }
        }, "Client-"+i);
    }

    public static void main(String args[]) {
        var ls = new ArrayList<Pair<Integer,Integer>>();
        ls.add(new Pair<>(1,2));
        ls.add(new Pair<>(3,4));
        ls.add(new Pair<>(5,6));
        RobotMonitor rm = new RobotMonitor(ls);
        Thread[] clients = new Thread[2];
        Thread[] robots = new Thread[3];
        for (int i = 0; i<clients.length; i++) clients[i] = generateClient(rm, i);
        for (int i = 0; i<robots.length; i++) robots[i] = generateRobot(rm, i);
        for (int i = 0; i<clients.length; i++) clients[i].start();
        for (int i = 0; i<robots.length; i++) robots[i].start();
    }

}
