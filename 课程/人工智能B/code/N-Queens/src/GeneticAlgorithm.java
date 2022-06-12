import java.util.*;
import java.util.stream.Collectors;

/**
 * @author 191181 高天翔
 * @date 2020.11.1
 */
public class GeneticAlgorithm {
    private static int n;
    private static final double MUTATE_RATE = 0.02;
    private static final int TIMES = 10000;

    public static void main(String[] args) {
        int queenNum = QueenTool.getNum();
        long startTime = System.nanoTime();
        run(queenNum);
        long endTime = System.nanoTime();
        System.out.println("genetic algorithm program run time： " + (endTime - startTime) / 1000000.0 + "ms");
    }

    public static void run(int queenNum) {
        n = queenNum;
        int[] father = init();
        int[] mother = init();

        for (int i = 0; i < TIMES; i++) {
            List<Integer[]> son = new ArrayList<>();
            for (int j = 0; j < 4 * n; j++) {
                son.add(Arrays.stream(crossover(father, mother)).boxed().toArray(Integer[]::new));
            }

            mutate(son);

            Map<Integer[], Integer> res = select(son);

            for (Map.Entry<Integer[], Integer> c : res.entrySet()) {
                if (c.getValue() == 0) {
                    System.out.println("Solution to " + n + " queens using genetic algorithm:");
                    QueenTool.printBoard(c.getKey());
                    return;
                }
            }

            father = Arrays.stream(res.entrySet().iterator().next().getKey()).mapToInt(k -> k).toArray();
            mother = Arrays.stream(res.entrySet().iterator().next().getKey()).mapToInt(k -> k).toArray();
        }

        System.out.println("Generations are more than the max number of iterations!");
    }

    /**
     * use List to shuffle and trans to array to return
     *
     * @return a random array of the queens chess board
     */
    private static int[] init() {
        List<Integer> list = new ArrayList<>();
        for (int i = 1; i <= n; i++) {
            list.add(i);
        }
        Collections.shuffle(list);
        return list.stream().mapToInt(i -> i).toArray();
    }

    /**
     * Chromosomal crossover for genetic recombination
     *
     * @param originFather father of the son
     * @param originMother mother of the son
     * @return the son
     */
    private static int[] crossover(int[] originFather, int[] originMother) {
        int[] father = Arrays.copyOf(originFather, n);
        int[] mother = Arrays.copyOf(originMother, n);
        int[] son = new int[n];

        int[] rend = new int[n];
        for (int i = 0; i < n; i++) {
            rend[i] = Math.random() >= 0.5 ? 0 : 1;
        }

        int f = 0, m = 0;
        for (int i = 0; i < n; i++) {
            // move the point of chromosomal
            if (rend[i] == 0) {
                for (f = 0; f < n; f++) {
                    if (father[f] != 0) {
                        break;
                    }
                }
            } else {
                for (m = 0; m < n; m++) {
                    if (mother[m] != 0) {
                        break;
                    }
                }
            }
            // Inherit parents' genes
            son[i] = rend[i] == 0 ? father[f] : mother[m];

            // Set the inherited genes to 0
            for (int j = 0; j < n; j++) {
                if (father[j] == son[i]) {
                    father[j] = 0;
                }
                if (mother[j] == son[i]) {
                    mother[j] = 0;
                }
            }
        }
        return son;
    }

    /**
     * mutate to suit for the env
     * for the chromosome in chromosomes
     * choose two gene randomly to mutate
     *
     * @param chromosomes chromosomes of the obj
     */
    private static void mutate(List<Integer[]> chromosomes) {
        for (Integer[] chromosome : chromosomes) {
            if (Math.random() > MUTATE_RATE) {
                continue;
            }
            int i = (int) (Math.random() * n);
            int j = (int) (Math.random() * n);
            if (i == j) {
                continue;
            }

            // three times xor to swap two numbers to hybridize
            chromosome[i] ^= chromosome[j];
            chromosome[j] ^= chromosome[i];
            chromosome[i] ^= chromosome[j];
        }
    }

    private static Integer fit(Integer[] result) {
        int res = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (Math.abs(result[i] - result[j]) == j - i || result[i].equals(result[j])) {
                    res++;
                }
            }
        }
        return res;
    }

    /**
     * method to choose the best individual
     *
     * @param son subgroup of the population
     * @return the best individual of the subgroup
     */
    private static Map<Integer[], Integer> select(List<Integer[]> son) {
        Map<Integer[], Integer> result = son.stream().collect(Collectors.toMap(x -> x, GeneticAlgorithm::fit));
        result = result.entrySet().stream().
                sorted(Map.Entry.comparingByValue()).
                collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, (oldValue, newValue) -> oldValue, LinkedHashMap::new));
        return result;
    }
}
