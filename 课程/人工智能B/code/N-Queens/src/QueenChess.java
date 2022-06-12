/**
 * Queen Chess Class
 *
 * @author 191181 高天翔
 * @date 2020.11.1
 */
public class QueenChess {
    public int row;
    public final int column;

    public QueenChess(int row, int column) {
        this.row = row;
        this.column = column;
    }

    /**
     * Move the queen one row down
     *
     * @param steps the steps chess move
     * @param n     the board size (input to prevent cross-border)
     */
    public void move(int steps, int n) {
        row = (row + steps + n) % n;
    }

    /**
     * @return the row of the chess
     */
    public int getRow() {
        return row;
    }

    /**
     * @return the column of the chess
     */
    public int getColumn() {
        return column;
    }
}