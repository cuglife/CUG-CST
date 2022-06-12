/**
 * @author 191181 高天翔
 * @date 2020.11.3
 */
public class Area {
    int row;
    int col;

    protected boolean hasGold;
    protected boolean hasPit;
    protected boolean isBreezy;
    protected boolean hasWumpus;
    protected boolean isStench;
    protected boolean hasWall;

    protected boolean isVisited;


    /**
     * init a blank place
     */
    public Area(int row, int col) {
        this.row = row;
        this.col = col;

        this.hasGold = false;
        this.hasPit = false;
        this.isBreezy = false;
        this.hasWumpus = false;
        this.isStench = false;
        this.hasWall = false;
        this.isVisited = false;
    }

    int getRow() {
        return this.row;
    }

    int getCol() {
        return this.col;
    }

    boolean getVisited() {
        return this.isVisited;
    }

    boolean getHasGold() {
        return this.hasGold;
    }

    boolean getHasPit() {
        return this.hasPit;
    }

    boolean getIsBreezy() {
        return this.isBreezy;
    }

    boolean getHasWumpus() {
        return this.hasWumpus;
    }

    boolean getIsStench() {
        return this.isStench;
    }

    boolean getHasWall() {
        return this.hasWall;
    }

    public void setVisited() {
        isVisited = true;
    }
}
