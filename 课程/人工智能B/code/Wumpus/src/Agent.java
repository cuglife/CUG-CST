import java.util.ArrayList;
import java.util.Arrays;
import java.util.Formatter;

/**
 * @author 191181 高天翔
 * @date 2020.11.3
 */
public class Agent {
    WumpusWorld world;
    int deathArea = 4;
    int unknownAreas;
    double acceptableRisk = 0.5;
    double[][] deathProbability;
    boolean[][] isMarked;
    ArrayList<Area> markedArea = new ArrayList<>();
    Area agentArea;
    int score = 0;
    int steps;
    boolean isAgentAlive = true;
    boolean isGiveUp = false;
    boolean isWumpusAlive = true;
    boolean isGrabGold = false;

    /**
     * the 4 different directions of the agent at a specific time(step)
     */
    final static int NORTH = 0;
    final static int EAST = 1;
    final static int SOUTH = 2;
    final static int WEST = 3;

    String[] printDirection = {"↑", "→", "↓", "←"};
    /**
     * default direction is east
     */
    private int direction = 1;

    /**
     * init agents
     */
    public Agent(WumpusWorld wumpusWorld) {
        this.world = wumpusWorld;
        int size = world.getSize();
        unknownAreas = size * size;
        deathProbability = new double[size + 2][size + 2];
        for (double[] arr : deathProbability) {
            Arrays.fill(arr, -1);
        }
        isMarked = new boolean[size + 2][size + 2];
        agentArea = wumpusWorld.getArea(size, 1);
        markArea(agentArea);
        updateDeathProbability();
    }


    /**
     * start auto playing the game
     */
    public void simulate() {
        if (!go(think())) {
            isGiveUp = true;
        }
    }


    // 搜寻 area 找死亡率小于50%的区域，未找到则放弃
    // 全局找完则同样放弃
    private int think() {
        int row = agentArea.getRow();
        int col = agentArea.getCol();
        double bestHeuristic = Integer.MAX_VALUE;
        int bestDirection = -1;
        if (!world.getArea(row - 1, col).getHasWall()
                && !world.getArea(row - 1, col).getVisited()
                && deathProbability[row - 1][col] < bestHeuristic) {
            bestHeuristic = deathProbability[row - 1][col];
            bestDirection = NORTH;
        }
        if (!world.getArea(row + 1, col).getHasWall()
                && !world.getArea(row + 1, col).getVisited()
                && deathProbability[row + 1][col] < bestHeuristic) {
            bestHeuristic = deathProbability[row + 1][col];
            bestDirection = SOUTH;
        }
        if (!world.getArea(row, col + 1).getHasWall()
                && !world.getArea(row, col + 1).getVisited()
                && deathProbability[row][col + 1] < bestHeuristic) {
            bestHeuristic = deathProbability[row][col + 1];
            bestDirection = EAST;
        }
        if (!world.getArea(row, col - 1).getHasWall()
                && !world.getArea(row, col - 1).getVisited()
                && deathProbability[row][col - 1] < bestHeuristic) {
            bestHeuristic = deathProbability[row][col - 1];
            bestDirection = WEST;
        }

        if (steps > world.getSize() * world.getSize() * 2.5) {
            isGiveUp = true;
        }

        return bestHeuristic >= acceptableRisk ? -1 : bestDirection;
    }

    public boolean go(int direction) {
        if (direction != -1) {
            if (direction == NORTH) {
                return goNorth();
            } else if (direction == EAST) {
                return goEast();
            } else if (direction == SOUTH) {
                return goSouth();
            } else if (direction == WEST) {
                return goWest();
            }
        } else {
            return goBack();
        }
        return false;
    }

    public void turnLeft() {
        this.direction = (direction - 1 + 4) % 4;
        score--;
        steps++;
    }

    public void turnRight() {
        this.direction = (direction + 1) % 4;
        score--;
        steps++;
    }

    public boolean moveForward() {
        score--;
        steps++;
        int row = agentArea.getRow();
        int col = agentArea.getCol();
        int rowStore = row;
        int colStore = col;
        if (direction == NORTH && !world.getArea(row - 1, col).getHasWall()) {
            row--;
        } else if (direction == SOUTH && !world.getArea(row + 1, col).getHasWall()) {
            row++;
        } else if (direction == EAST && !world.getArea(row, col + 1).getHasWall()) {
            col++;
        } else if (direction == WEST && !world.getArea(row, col - 1).getHasWall()) {
            col--;
        }
        // can't move or death probability > acceptableRisk, then give up
        if (row == rowStore && col == colStore) {
            return false;
        } else if (deathProbability[row][col] >= acceptableRisk) {
            return false;
        } else {
            agentArea = world.getArea(row, col);
            if (agentArea.getHasGold()) {
                grabGold();
                // Path.goBack(this, world.getArea(world.getSize(), 1), agentArea);
            }
            markArea(agentArea);
            return true;
        }
    }

    public boolean goEast() {
        if (direction != EAST) {
            if (direction == SOUTH) {
                turnLeft();
            } else if (direction == WEST) {
                turnRight();
                turnRight();
            } else if (direction == NORTH) {
                turnRight();
            }
        }
        return moveForward();
    }

    public boolean goWest() {
        if (direction != WEST) {
            if (direction == NORTH) {
                turnRight();
            } else if (direction == EAST) {
                turnRight();
                turnRight();
            } else if (direction == SOUTH) {
                turnLeft();
            }
        }
        return moveForward();
    }

    public boolean goNorth() {
        if (direction != NORTH) {
            if (direction == WEST) {
                turnRight();
            } else if (direction == SOUTH) {
                turnRight();
                turnRight();
            } else if (direction == EAST) {
                turnLeft();
            }
        }
        return moveForward();
    }

    public boolean goSouth() {
        if (direction != SOUTH) {
            if (direction == WEST) {
                turnLeft();
            } else if (direction == NORTH) {
                turnRight();
                turnRight();
            } else if (direction == EAST) {
                turnRight();
            }
        }
        return moveForward();
    }

    public boolean goBack() {
        world.setHasWall(agentArea, true);
        if (direction == NORTH) {
            return goSouth();
        } else if (direction == WEST) {
            return goEast();
        } else if (direction == SOUTH) {
            return goNorth();
        } else if (direction == EAST) {
            return goWest();
        }
        return false;
    }

    public void goBackHome() {
        int size = world.getSize();
        Area start = world.getArea(size, 1);
        Area end = agentArea;
        int returnSteps = (Math.abs(start.getRow() - end.getRow()) + Math.abs(start.getCol() - end.getCol()));
        score -= returnSteps;
        agentArea = start;
    }

    public void markArea(Area area) {
        int row = agentArea.getRow();
        int col = agentArea.getCol();
        if (!agentArea.getVisited()) {
            agentArea.setVisited();
            isMarked[agentArea.getRow()][agentArea.getCol()] = true;
            deathProbability[row][col] = 0;
            unknownAreas = updateMap();
        }
        if (area.getIsBreezy()) {
            markedArea.add(agentArea);
        }
        if (area.getIsStench()) {
            if (!chooseDirectionToShoot()) {
                markedArea.add(agentArea);
            }
        }
        if (!area.getIsBreezy() && !area.getIsStench()) {
            markAreaSafe(row, col);
        }

        markAreaDangerous();
        updateDeathProbability();
    }


    private void markAreaDangerous() {
        for (Area area : markedArea) {
            markAreaDangerous(area);
        }
    }

    private void markAreaDangerous(Area area) {
        int row = area.getRow();
        int col = area.getCol();
        int unknownNeighbors = getUnknownNeighbors(row, col);
        double deathProbabilityInArea = unknownNeighbors == 0 ? 0 : 1.0 / unknownNeighbors;

        if (!world.getArea(row + 1, col).getVisited() && deathProbability[row + 1][col] != 0 && deathProbability[row][col] != 1) {
            markNeighbor(row + 1, col, deathProbabilityInArea);
        }
        if (!world.getArea(row - 1, col).getVisited() && deathProbability[row - 1][col] != 0 && deathProbability[row][col] != 1) {
            markNeighbor(row - 1, col, deathProbabilityInArea);
        }
        if (!world.getArea(row, col - 1).getVisited() && deathProbability[row][col - 1] != 0 && deathProbability[row][col] != 1) {
            markNeighbor(row, col - 1, deathProbabilityInArea);
        }
        if (!world.getArea(row, col + 1).getVisited() && deathProbability[row][col + 1] != 0 && deathProbability[row][col] != 1) {
            markNeighbor(row, col + 1, deathProbabilityInArea);
        }

    }

    private void markAreaSafe(int row, int col) {
        deathProbability[row][col] = 0;
        deathProbability[row + 1][col] = 0;
        deathProbability[row - 1][col] = 0;
        deathProbability[row][col + 1] = 0;
        deathProbability[row][col - 1] = 0;

        isMarked[row][col] = true;
        isMarked[row + 1][col] = true;
        isMarked[row - 1][col] = true;
        isMarked[row][col - 1] = true;
        isMarked[row][col + 1] = true;
    }

    private void updateDeathProbability() {
        unknownAreas = updateMap();
        int size = world.getSize();
        double deathPossibilityInArea = deathArea / (unknownAreas * 1.0);

        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                if (!isMarked[i][j] && deathProbability[i][j] != 0 && deathProbability[i][j] != 1) {
                    deathProbability[i][j] = deathPossibilityInArea;
                }
            }
        }
    }

    private int getUnknownNeighbors(int row, int col) {
        int unknownNeighbors = 4;
        if (deathProbability[row - 1][col] == 0 || world.getArea(row - 1, col).getHasWall()) {
            unknownNeighbors--;
        }
        if (deathProbability[row + 1][col] == 0 || world.getArea(row + 1, col).getHasWall()) {
            unknownNeighbors--;
        }
        if (deathProbability[row][col + 1] == 0 || world.getArea(row, col + 1).getHasWall()) {
            unknownNeighbors--;
        }
        if (deathProbability[row][col - 1] == 0 || world.getArea(row, col - 1).getHasWall()) {
            unknownNeighbors--;
        }
        return unknownNeighbors;
    }

    private void markNeighbor(int row, int col, double deathProbabilityInArea) {
//            if (isMarked[row][col]) {
//                deathProbability[row][col] = (deathProbabilityInArea + deathProbability[row][col] >= 1) ? 1 : deathProbability[row][col] + deathProbabilityInArea;
//            } else {
//                deathProbability[row][col] = deathProbabilityInArea;
//                isMarked[row][col] = true;
//            }


        deathProbability[row][col] = deathProbabilityInArea;
        isMarked[row][col] = true;

    }

    private int updateMap() {
        int unknownAreasTmp = 0;
        int dangerArea = 3 + (isWumpusAlive ? 1 : 0);
        int size = world.getSize();
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                if (deathProbability[i][j] != 0 && deathProbability[i][j] != 1) {
                    unknownAreasTmp++;
                }
                if (deathProbability[i][j] == 1) {
                    dangerArea--;
                }
            }
        }
        if (unknownAreasTmp == 0) {
            unknownAreasTmp = Integer.MAX_VALUE;
        }
        deathArea = dangerArea;
        return unknownAreasTmp;
    }

    /**
     * prepare to optimize the probability of hitting wumpus
     * undo
     */
    public boolean chooseDirectionToShoot() {
        if (isWumpusAlive()) {
            return shootWumpus();
        }
        return true;
    }

    public boolean shootWumpus() {
        System.out.println("try to shoot the wumpus");
        score -= 10;
        int row = agentArea.getRow();
        int col = agentArea.getCol();
        if (direction == NORTH) {
            for (int i = row; i >= 1; i--) {
                if (world.getArea(i, col).getHasWumpus()) {
                    wumpusScream(world.getArea(i, col));
                    return true;
                }
            }
        } else if (direction == SOUTH) {
            for (int i = row; i <= world.getSize(); i++) {
                if (world.getArea(i, col).getHasWumpus()) {
                    wumpusScream(world.getArea(i, col));
                    return true;
                }
            }
        } else if (direction == WEST) {
            for (int i = col; i >= 1; i--) {
                if (world.getArea(row, i).getHasWumpus()) {
                    wumpusScream(world.getArea(row, i));
                    return true;
                }
            }
        } else if (direction == EAST) {
            for (int i = col; i <= world.getSize(); i++) {
                if (world.getArea(row, i).getHasWumpus()) {
                    wumpusScream(world.getArea(row, i));
                    return true;
                }
            }
        }
        System.out.println("Kill failed");
        return false;
    }

    public void grabGold() {
        score += 1000;
        world.setHasGold(agentArea, false);
        isGrabGold = true;
    }

    public boolean isAlive() {
        boolean isAreaDangerous = (agentArea.getHasPit() || (agentArea.getHasWumpus() && isWumpusAlive()));
        if (isAreaDangerous && isAgentAlive) {
            score -= 1000;
            isAgentAlive = false;
        }
        return isAgentAlive;
    }

    public boolean isGiveUp() {
        return isGiveUp;
    }

    public boolean isGrabGold() {
        return isGrabGold;
    }

    public boolean isWumpusAlive() {
        return isWumpusAlive;
    }

    public void wumpusScream(Area wumpusArea) {
        isWumpusAlive = false;
        world.setHasWumpus(wumpusArea, false);
        deathArea--;
        System.out.println("kill successful!");
    }

    public void setAcceptableRisk(double acceptableRisk) {
        this.acceptableRisk = acceptableRisk;
    }

    public void printDeathPossibility() {
        int size = world.getSize();
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                System.out.print(new Formatter().format("%.2f", deathProbability[i][j]).toString() + "  ");
            }
            System.out.println();
        }
        System.out.println();
    }

    public void printWorld(boolean isKnown) {
        int size = world.getSize();
        StringBuilder[][] printStrings = new StringBuilder[size][size];

        for (int i = 0; i < printStrings.length; i++) {
            for (int j = 0; j < printStrings[i].length; j++) {
                printStrings[i][j] = new StringBuilder();
            }
        }

        printStrings[agentArea.getRow() - 1][agentArea.getCol() - 1].append("A").append(printDirection[direction]);

        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                if (world.getArea(i, j).getVisited() || isKnown) {
                    if (world.getArea(i, j).getHasPit()) {
                        printStrings[i - 1][j - 1].append("X");
                    }
                    if (world.getArea(i, j).getIsBreezy()) {
                        printStrings[i - 1][j - 1].append("~");
                    }
                    if (world.getArea(i, j).getIsStench()) {
                        printStrings[i - 1][j - 1].append("*");
                    }
                    if (world.getArea(i, j).getHasGold()) {
                        printStrings[i - 1][j - 1].append("$");
                    }
                    if (world.getArea(i, j).getHasWumpus() && isWumpusAlive()) {
                        printStrings[i - 1][j - 1].append("W");
                    }
                }
                if (isKnown || world.getArea(i, j).getVisited()) {
                    if (printStrings[i - 1][j - 1].length() == 0) {
                        printStrings[i - 1][j - 1].append("____");
                    } else if (printStrings[i - 1][j - 1].length() == 1) {
                        printStrings[i - 1][j - 1].append("___");
                    } else if (printStrings[i - 1][j - 1].length() == 2) {
                        printStrings[i - 1][j - 1].append("__");
                    } else if (printStrings[i - 1][j - 1].length() == 3) {
                        printStrings[i - 1][j - 1].append("_");
                    }
                } else {
                    if (printStrings[i - 1][j - 1].length() == 0) {
                        printStrings[i - 1][j - 1].append("????");
                    } else if (printStrings[i - 1][j - 1].length() == 1) {
                        printStrings[i - 1][j - 1].append("???");
                    } else if (printStrings[i - 1][j - 1].length() == 2) {
                        printStrings[i - 1][j - 1].append("??");
                    } else if (printStrings[i - 1][j - 1].length() == 3) {
                        printStrings[i - 1][j - 1].append("?");
                    }
                }


            }

        }

        for (StringBuilder[] row : printStrings) {
            for (StringBuilder chess : row) {
                System.out.print(chess + "  ");
            }
            System.out.println();
        }
        System.out.println();
    }

    public void printWorld() {
        System.out.println("real world: ");
        printWorld(true);
        System.out.println("agent world: ");
        printWorld(false);
        System.out.println("death possibility: ");
        printDeathPossibility();
    }

}

