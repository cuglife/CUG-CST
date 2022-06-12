import java.util.Random;
import java.util.Scanner;

/**
 * @author 191181 高天翔
 * @date 2020.11.1
 */
public class QueenTool {

    public static int getNum() {
        int n;
        Scanner s = new Scanner(System.in);
        while (true) {
            System.out.println("Enter the number of Queens :");
            n = s.nextInt();
            if (n == 2 || n == 3) {
                System.out.println("No Solution possible for " + n + " Queens. Please enter another number");
            } else {
                return n;
            }
        }
    }

    public static int getSelect() {
        int n;
        Scanner s = new Scanner(System.in);
        while (true) {
            System.out.println("please select an algorithm to solve n queens problem: ");
            System.out.println("1: HillClimbingRandomRestart");
            System.out.println("2: MinConflictsSolver");
            System.out.println("3: GeneticAlgorithm");
            n = s.nextInt();
            if (n != 1 && n != 2 && n != 3) {
                System.out.println("Input error, Please enter number from 1 to 3 ");
            } else {
                return n;
            }
        }
    }

    /**
     * Method to create a new random board
     */
    public static QueenChess[] generateBoard(int n) {
        QueenChess[] startBoard = new QueenChess[n];
        for (int i = 0; i < n; i++) {
            startBoard[i] = new QueenChess(i, i);
        }
        // shuffle
        for (int i = 0; i <= n - 2; i++) {
            int x = (int) (Math.random() * n);
            // swap
            int tmpRow = startBoard[i].row;
            startBoard[i].row = startBoard[x].row;
            startBoard[x].row = tmpRow;
        }

        return startBoard;
    }


    /**
     * Method to find Heuristics of a state
     *
     * @param board board distribution
     * @return the heuristic of the current chess board
     */
    public static int findHeuristic(QueenChess[] board) {
        int conflicts = 0;

        int n = board.length;
        int[] rowFrequency = new int[n];
        int[] mainDiagFrequency = new int[2 * n];
        int[] secondaryDiagFrequency = new int[2 * n];

        for (int i = 0; i < n; i++) {
            rowFrequency[board[i].getRow()] += 1;
            mainDiagFrequency[board[i].getRow() + i] += 1;
            secondaryDiagFrequency[n - board[i].getRow() + i] += 1;
        }

        for (int i = 0; i < 2 * n; i++) {

            if (i < n) {
                conflicts += (rowFrequency[i] * (rowFrequency[i] - 1)) / 2;
            }
            conflicts += (mainDiagFrequency[i] * (mainDiagFrequency[i] - 1)) / 2;
            conflicts += (secondaryDiagFrequency[i] * (secondaryDiagFrequency[i] - 1)) / 2;
        }
        return conflicts;
    }

    /**
     * Method to print the Current Board State
     *
     * @param boardState board distribution
     */
    public static void printBoard(QueenChess[] boardState) {
        //Creating temporary board from the present board
        int n = boardState.length;
        int[][] tempBoard = new int[n][n];
        for (QueenChess queenChess : boardState) {
            //Get the positions of Queen from the Present board and set those positions as 1 in temp board
            tempBoard[queenChess.getRow()][queenChess.getColumn()] = 1;
        }
        QueenTool.printBoard(tempBoard);
    }

    public static void printBoard(int[][] chessBoard) {
        char printChess;
        System.out.println();
        for (int[] chessInRow : chessBoard) {
            for (int chess : chessInRow) {
                printChess = chess == 0 ? '-' : '#';
                System.out.print(printChess + "  ");
            }
            System.out.println();
        }
        System.out.println();
    }

    public static void printBoard(Integer[] chessBoard) {
        int n = chessBoard.length;
        //Creating temporary board from the present board
        int[][] tempBoard = new int[n][n];
        for (int i = 0; i < n; i++) {
            //Get the positions of Queen from the Present board and set those positions as 1 in temp board
            tempBoard[i][chessBoard[i] - 1] = 1;
        }
        QueenTool.printBoard(tempBoard);
    }
}
