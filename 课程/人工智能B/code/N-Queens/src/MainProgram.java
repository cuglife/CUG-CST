import java.util.Scanner;

/**
 * @author 191181
 * @date 2020.11.3
 */
public class MainProgram {
    public static void main(String[] args) {
        int n = QueenTool.getNum();
        int select = QueenTool.getSelect();
        long startTime = System.nanoTime();
        if (select == 1) {
            HillClimbingRandomRestart.run(n);
        } else if (select == 2) {
            MinConflictsSolver.run(n);
        } else if (select == 3) {
            GeneticAlgorithm.run(n);
        } else {
            System.out.println("something error!");
        }
        long endTime = System.nanoTime();
        System.out.println("algorithm you choose run time： " + (endTime - startTime) / 1000000.0 + "ms");

        System.out.println("program run over, press enter to terminate ");
        Scanner scanner = new Scanner(System.in);
        if (scanner.nextLine() != null) {
            System.out.println("bye bye");
        }
    }
}
