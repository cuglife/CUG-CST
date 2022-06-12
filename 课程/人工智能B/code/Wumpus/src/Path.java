/**
 * @author 191181 高天翔
 * @date 2020.11.3
 */

import java.util.*;
import java.util.Random;
import java.util.Stack;

public class Path {

    /**
     * stupid back way
     */
    public static void goBack(Agent agent, Area start, Area end) {
        agent.score -= agent.steps;
    }

    private int heuristic(Area start, Area end) {
        return 10 * (int) (Math.abs(start.getRow() - end.getRow()) + Math.abs(start.getCol() - end.getCol()));
    }

}


