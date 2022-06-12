//Program to implement Hill Climbing with random restart to solve N-queens problem

/**
 * Program to implement Hill Climbing with random restart to solve N-queens problem
 *
 * @author 191181 高天翔
 * @date 2020.11.1
 */
public class HillClimbingRandomRestart {
    private static int n;
    private static int stepsClimbedAfterLastRestart = 0;
    private static int totalStepsClimbed = 0;
    private static int heuristic = 0;
    private static int randomRestarts = 0;
    private static final int MAX_STEPS = 10000000;

    public static void main(String[] args) {
        int queenNum = QueenTool.getNum();
        long startTime = System.nanoTime();
        run(queenNum);
        long endTime = System.nanoTime();
        System.out.println("hill climbing random restart program run time： " + (endTime - startTime) / 1000000.0 + "ms");
    }

    public static void run(int queenNum) {
        n = queenNum;
        int presentHeuristic;
        // creating the initial Board
        QueenChess[] presentBoard = QueenTool.generateBoard(n);

        // test if the present board is the solution board
        presentHeuristic = QueenTool.findHeuristic(presentBoard);
        while ((presentHeuristic != 0) && (totalStepsClimbed < MAX_STEPS)) {
            // get the next board
            presentBoard = hillClimbing(presentBoard);
            presentHeuristic = heuristic;
        }

        if (totalStepsClimbed < MAX_STEPS) {
            //Printing the solution
            System.out.println("Solution to " + n + " queens using hill climbing with random restart:");
            QueenTool.printBoard(presentBoard);
            System.out.println("\nTotal number of Steps Climbed: " + totalStepsClimbed);
            System.out.println("Number of random restarts: " + randomRestarts);
            System.out.println("Steps Climbed after last restart: " + stepsClimbedAfterLastRestart);
        } else {
            System.out.println("Total Steps is bigger than max Steps!");
        }
    }


    /**
     * Method to get the next board with lower heuristic
     *
     * @param presentBoard p
     * @return lower heuristic chess board
     */
    public static QueenChess[] hillClimbing(QueenChess[] presentBoard) {

        QueenChess[] nextBoard = new QueenChess[n];
        QueenChess[] tmpBoard = new QueenChess[n];
        int presentHeuristic = QueenTool.findHeuristic(presentBoard);
        int bestHeuristic = presentHeuristic;
        int tempHeuristic;

        //  Copy present board as best board and temp board
        for (int i = 0; i < n; i++) {
            nextBoard[i] = new QueenChess(presentBoard[i].getRow(), presentBoard[i].getColumn());
            tmpBoard[i] = nextBoard[i];
        }


        //  Iterate each column
        for (int i = 0; i < n; i++) {
            // recovery the previous column and traverse the current column to find the function with the smallest heuristic value
            int col = (i - 1 + n) % n;
            tmpBoard[col] = new QueenChess(presentBoard[col].getRow(), presentBoard[col].getColumn());
            tmpBoard[i] = new QueenChess(0, tmpBoard[i].getColumn());

            //  Iterate each row
            for (int j = 0; j < n; j++) {
                tempHeuristic = QueenTool.findHeuristic(tmpBoard);
                if (tempHeuristic < bestHeuristic) {
                    bestHeuristic = tempHeuristic;
                    for (int k = 0; k < n; k++) {
                        nextBoard[k] = new QueenChess(tmpBoard[k].getRow(), tmpBoard[k].getColumn());
                    }
                }

                //Move the queen
                if (tmpBoard[i].getRow() != n - 1) {
                    tmpBoard[i].move(1, n);
                }
            }
        }


        //Check whether the present bord and the best board found have same heuristic
        //Then randomly generate new board and assign it to best board
        if (bestHeuristic == presentHeuristic) {
            randomRestarts++;
            stepsClimbedAfterLastRestart = 0;
            nextBoard = QueenTool.generateBoard(n);
            heuristic = QueenTool.findHeuristic(nextBoard);
        } else {
            heuristic = bestHeuristic;
        }

        totalStepsClimbed++;
        stepsClimbedAfterLastRestart++;

        return nextBoard;
    }

}
