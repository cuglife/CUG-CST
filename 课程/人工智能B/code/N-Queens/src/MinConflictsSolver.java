/**
 * Program to implement Min Conflicts to solve N-queens problem
 *
 * @author 191181 高天翔
 * @date 2020.11.1
 */
public class MinConflictsSolver {
    private static int n;
    private static int totalStepsMoved = 0;
    private static final int MAX_STEPS = 100000;


    /**
     * row[i] shows the queen of the i row，col[i] shows the queen of the i col
     * mainDiag[i] shows the queen of the i main diag
     * secondaryDiag[i] shows the queen of the i secondary diag
     * <p>
     * Additional explanation
     * The row-col values on the diagonal from top left to bottom right are the same，use this as the index of the diag
     * Plus n - 1 to avoid the value is negative value
     */

    static int[] row;
    static int[] col;
    static int[] mainDiag;
    static int[] secondaryDiag;


    public static void main(String[] args) {
        int queenNum = QueenTool.getNum();
        long startTime = System.nanoTime();
        run(queenNum);
        long endTime = System.nanoTime();
        System.out.println("min conflicts program run time： " + (endTime - startTime) / 1000000.0 + "ms");
    }

    public static void run(int queenNum) {
        n = queenNum;
        row = new int[n];
        col = new int[n];
        mainDiag = new int[2 * n];
        secondaryDiag = new int[2 * n];

        System.out.println("n is: " + n);
        // creating the initial Board
        QueenChess[] board = QueenTool.generateBoard(n);
        update(board, row, col, mainDiag, secondaryDiag);

        int heuristic = QueenTool.findHeuristic(board);
        if (heuristic != 0) {
            minConflictsSolve(board);
        }

        if (totalStepsMoved < MAX_STEPS) {
            //Printing the solution
            System.out.println("Solution to " + n + " queens using min conflicts Solver");
            QueenTool.printBoard(board);
            System.out.println("Total number of Steps Moved: " + totalStepsMoved);
        } else {
            System.out.println("Total Steps is bigger than max Steps!");
        }
    }


    public static void minConflictsSolve(QueenChess[] board) {
        boolean isGood = false;
        while (!isGood && totalStepsMoved < MAX_STEPS) {
            //  Iterate each column
            for (int i = 0; i < n; i++) {
                totalStepsMoved++;
                if (adjustRow(i, board)) {
                    isGood = true;
                    break;
                }
            }
        }
    }

    /**
     * @param column adjust row for this column
     * @return is adjust good
     */
    public static boolean adjustRow(int column, QueenChess[] board) {
        // 通过列得到行
        int curRow = board[column].getRow();
        // 最佳行号，设置为当前行，然后更新
        int bestRow = curRow;

        // row 不减一的原因是交换任然保证每行每列都有一个而已
        // 现在还没移过去
        // 对角线冲突数为当前对角线皇后数减一
        int minConflict =
                row[bestRow]
                        + mainDiag[getP(bestRow, column, n)] - 1
                        + secondaryDiag[getC(bestRow, column)] - 1;

        // Iterate through each row
        for (int i = 0; i < n; i++) {
            if (i == curRow) {
                continue;
            }
            int conflict = row[i] + mainDiag[getP(i, column, n)] + secondaryDiag[getC(i, column)];
            // update data
            if (conflict < minConflict) {
                minConflict = conflict;
                bestRow = i;
            }
        }

        // update col, mainDiag, secondaryDiag
        if (bestRow != curRow) {
            // minus current row queen
            row[curRow]--;
            mainDiag[getP(curRow, column, n)]--;
            secondaryDiag[getC(curRow, column)]--;

            // add best row queen
            row[bestRow]++;
            mainDiag[getP(bestRow, column, n)]++;
            secondaryDiag[getC(bestRow, column)]++;

            // 注意，这里我们没有移动 在这个列上的之前的那个行上的皇后
            // move chess
            board[column].row = bestRow;
            // return is this col move successful
            if (row[curRow] == 1 && row[bestRow] == 1
                    && mainDiag[getP(bestRow, column, n)] == 1
                    && secondaryDiag[getC(bestRow, column)] == 1) {
                return satisfy(board);
            }
        }

        // any else
        return false;
    }

    public static boolean satisfy(QueenChess[] board) {
        // Iterate through each row
        for (int i = 0; i < n; i++) {
            if (row[board[i].getRow()] != 1 ||
                    mainDiag[getP(board[i].getRow(), i, n)] != 1 ||
                    secondaryDiag[getC(board[i].getRow(), i)] != 1) {
                return false;
            }
        }
        return true;
    }

    public static void update(QueenChess[] board, int[] row, int[] col, int[] mainDiag, int[] secondaryDiag) {
        int n = row.length;
        // 每列恰好一个皇后
        // 行后续再做初始化
        for (int point = 0; point < n; point++) {
            col[point] = 1;
            row[point] = 0;
        }
        // 初始化 diag 皇后数
        for (int point = 0; point < 2 * n - 1; point++) {
            mainDiag[point] = 0;
            secondaryDiag[point] = 0;
        }

        for (int i = 0; i < n; i++) {
            row[board[i].getRow()]++;
            mainDiag[getP(board[i].getRow(), board[i].getColumn(), n)]++;
            secondaryDiag[getC(board[i].getRow(), board[i].getColumn())]++;
        }
    }

    /**
     * 给定二维矩阵的一个点坐标
     * 返回其对应的右上到左下的对角线编号
     *
     * @return the index of the Diag
     */
    public static int getP(int row, int column, int n) {
        return row - column + n - 1;
    }

    public static int getC(int row, int column) {
        return row + column;
    }

}

