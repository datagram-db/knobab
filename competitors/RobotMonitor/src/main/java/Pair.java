import java.util.Comparator;
import java.util.Objects;

public class Pair<K, V>  {
    final public K key;
    final public V value;

    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
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


    public static class IntPairComparator implements Comparator<Pair<Integer, Integer>> {

        @Override
        public int compare(Pair<Integer, Integer> o1, Pair<Integer, Integer> o2) {
            if (Objects.equals(o1, o2)) return 0;
            else if (o1 == null) return -1;
            else if (o2 == null) return 1;
            else {
                var cmp = o1.key - o2.key;
                return cmp == 0 ?
                        o1.value - o2.value :
                        cmp;
            }
        }
    }
}
