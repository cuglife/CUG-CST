/**
 * @author 191181 高天翔
 * @date 2020.11.3
 */
public class WumpusWorld {

    public final Area[][] map;
    private int size;

    private Agent agent;

    private double pitProbability;

    /**
     * create a default world
     */
    public WumpusWorld() {
        this.size = 4;
        map = new Area[size + 2][size + 2];
        for (int i = 0; i < (size + 2); i++) {
            for (int j = 0; j < (size + 2); j++) {
                map[i][j] = new Area(i, j);
            }
        }

        for (int i = 0; i < size + 2; i++) {
            setHasWall(map[0][i], true);
            setHasWall(map[size + 1][i], true);
            setHasWall(map[i][0], true);
            setHasWall(map[i][size + 1], true);
        }

        setHasWumpus(map[2][1], true);
        setHasPit(map[1][4], true);
        setHasPit(map[2][3], true);
        setHasPit(map[4][3], true);
        setHasGold(map[2][2], true);

//        setHasWumpus(map[3][2], true);
//        setHasPit(map[4][3], true);
//        setHasPit(map[3][4], true);
//        setHasPit(map[4][4], true);
//        setHasGold(map[2][4], true);
    }

    /**
     * create a random world
     *
     * @param size           the size of the board grid
     * @param pitProbability the probability of the pit appear in one area
     */
    public WumpusWorld(int size, double pitProbability) {
        this.size = size;
        this.pitProbability = pitProbability;
        map = new Area[size + 2][size + 2];
        for (int i = 0; i < (size + 2); i++) {
            for (int j = 0; j < (size + 2); j++) {
                map[i][j] = new Area(i, j);
            }
        }

        for (int i = 0; i < size + 2; i++) {
            setHasWall(map[0][i], true);
            setHasWall(map[size + 1][i], true);
            setHasWall(map[i][0], true);
            setHasWall(map[i][size + 1], true);
        }

        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                setHasPit(map[i][j], Math.random() < pitProbability);
            }
        }


        while (true) {
            int wumpusRow = 1 + (int) (Math.random() * size);
            int wumpusCol = 1 + (int) (Math.random() * size);
            if (wumpusRow != size || wumpusCol != 1) {
                setHasWumpus(map[wumpusRow][wumpusCol], true);
                break;
            }
        }

        while (true) {
            int goldRow = 1 + (int) (Math.random() * size);
            int goldCol = 1 + (int) (Math.random() * size);
            if (goldRow != size || goldCol != 1) {
                setHasGold(map[goldRow][goldCol], true);
                break;
            }
        }
    }

    /**
     * @param size the size of the board grid
     * @param pits the num of pits in the world
     */
    public WumpusWorld(int size, int pits) {
        this.size = size;
        this.pitProbability = pits / (size * size + 0.0);
        map = new Area[size + 2][size + 2];
        for (int i = 0; i < (size + 2); i++) {
            for (int j = 0; j < (size + 2); j++) {
                map[i][j] = new Area(i, j);
            }
        }

        for (int i = 0; i < size + 2; i++) {
            setHasWall(map[0][i], true);
            setHasWall(map[size + 1][i], true);
            setHasWall(map[i][0], true);
            setHasWall(map[i][size + 1], true);
        }

        while (true) {
            int wumpusRow = 1 + (int) (Math.random() * size);
            int wumpusCol = 1 + (int) (Math.random() * size);
            if (wumpusRow != size || wumpusCol != 1) {
                setHasWumpus(map[wumpusRow][wumpusCol], true);
                break;
            }
        }

        for (int i = 0; i < pits; ) {
            while (true) {
                int pitRow = 1 + (int) (Math.random() * size);
                int pitCol = 1 + (int) (Math.random() * size);
                boolean isNotStartArea = pitRow != size || pitCol != 1;
                if (isNotStartArea && !map[pitRow][pitCol].getHasPit() && !map[pitRow][pitCol].getHasWumpus()) {
                    //System.out.println("xxxxxxxxxx  " + pitRow + "  " + pitCol);
                    setHasPit(map[pitRow][pitCol], true);
                    i++;
                    break;
                }
            }
        }

        while (true) {
            int goldRow = 1 + (int) (Math.random() * size);
            int goldCol = 1 + (int) (Math.random() * size);
            boolean isNotStartArea = goldRow != size || goldRow != 1;
            if (isNotStartArea&& !map[goldRow][goldCol].getHasPit() && !map[goldRow][goldCol].getHasWumpus()) {
                setHasGold(map[goldRow][goldCol], true);
                break;
            }
        }
    }

    public int getSize() {
        return size;
    }

    public double getPitProbability() {
        return pitProbability;
    }

    public Area getArea(int row, int col) {
        return map[row][col];
    }

    void setHasGold(Area area, boolean value) {
        area.hasGold = value;
    }

    void setHasPit(Area area, boolean value) {
        if (value && !area.getHasWall()) {
            int row = area.getRow();
            int col = area.getCol();
            area.hasPit = value;
            if (!map[row][col + 1].getHasWall()) {
                setIsBreezy(map[row][col + 1], true);
            }
            if (!map[row + 1][col].getHasWall()) {
                setIsBreezy(map[row + 1][col], true);
            }
            if (!map[row][col - 1].getHasWall()) {
                setIsBreezy(map[row][col - 1], true);
            }
            if (!map[row - 1][col].getHasWall()) {
                setIsBreezy(map[row - 1][col], true);
            }
        }
    }

    void setIsBreezy(Area area, boolean value) {
        area.isBreezy = value;
    }

    void setHasWumpus(Area area, boolean value) {
        if (!area.getHasWall()) {
            int row = area.getRow();
            int col = area.getCol();
            area.hasWumpus = value;
            if (!map[row][col + 1].getHasWall()) {
                setIsSmelly(map[row][col + 1], value);
            }
            if (!map[row + 1][col].getHasWall()) {
                setIsSmelly(map[row + 1][col], value);
            }
            if (!map[row][col - 1].getHasWall()) {
                setIsSmelly(map[row][col - 1], value);
            }
            if (!map[row - 1][col].getHasWall()) {
                setIsSmelly(map[row - 1][col], value);
            }
        }
    }

    void setIsSmelly(Area area, boolean value) {
        area.isStench = value;
    }

    void setHasWall(Area area, boolean value) {
        area.hasWall = value;
    }
}
