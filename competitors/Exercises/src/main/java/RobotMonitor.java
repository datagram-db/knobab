import java.util.*;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class OfficeHoursMonitor {

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
    }

    private Lock monitor;
    private Map<String, Pair<Integer,Integer>> moveRobotTo;
    private List<Pair<Integer,Integer>> robotPosition;
    private List<Pair<Integer, Integer>> positionsToGo;
    private Condition waitPosition;
    private int waiting_robots;

    public OfficeHoursMonitor(List<Pair<Integer,Integer>> robotInitialPosition) {
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
            System.out.println(dis+" starting to request position: ("+X+","+"Y)");
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
                if (waitPosition.)
            } else {
                hasValue = false;
            }
        } finally {
            monitor.unlock();
        }
        return hasValue;
    }

    public Pair<Integer, Integer> getRequest() {
        monitor.lock();
        try {
            var dis = Thread.currentThread().getName();
            if (positionsToGo.isEmpty())  {
                try {
                    waiting_robots++;
                    waitPosition.await();
                    waiting_robots--;
                } catch (InterruptedException e) { }
            }
            

        } finally {
            monitor.unlock();
        }
    }

}
