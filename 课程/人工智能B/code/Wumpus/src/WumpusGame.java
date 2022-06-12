import java.util.Scanner;

/**
 * @author 191181 高天翔
 * @date 2020.11.4
 */


public class WumpusGame {
    public static void main(String[] args) {
        // testAlgorithm();
        playGame(false, true);
        // getBestRiskRate();
    }

    public static void playGame(boolean isWorldRandom, boolean isAuto) {
        WumpusWorld wumpusWorld;

        if (isWorldRandom) {
            wumpusWorld = new WumpusWorld(4, 3);
        } else {
            wumpusWorld = new WumpusWorld();
        }

        Agent agent = new Agent(wumpusWorld);

        // agent.setAcceptableRisk(0.33);

        System.out.println("Game start, enjoy! accept risk rate: " + agent.acceptableRisk);

        agent.printWorld();
        if (isAuto) {
            computerGame(agent);
        } else {
            manualGame(agent);
        }

        if (!agent.isAlive()) {
            System.out.println("you lose, agent is dead! the score is: " + agent.score + ", steps you take: " + agent.steps);
        } else if (agent.isGrabGold()) {
            System.out.println("you win, the score is: " + agent.score + ", steps you take: " + agent.steps);
        } else if (agent.isGiveUp()) {
            System.out.println("agent give up, the score is: " + agent.score + ", steps you take: " + agent.steps);
        }

        System.out.println("game over, press enter to terminate ");
        Scanner scanner = new Scanner(System.in);

        if (scanner.nextLine() != null) {
            System.out.println("bye bye");
        }

    }

    public static void manualGame(Agent agent) {
        while (agent.isAlive() && !agent.isGrabGold()) {
            Scanner scanner = new Scanner(System.in);
            String action = scanner.nextLine();
            if ("a".equals(action)) {
                agent.turnLeft();
            } else if ("d".equals(action)) {
                agent.turnRight();
            } else if ("w".equals(action)) {
                agent.moveForward();
            } else if ("s".equals(action)) {
                if (agent.shootWumpus()) {
                    System.out.println("wumpus was killed!");
                } else {
                    System.out.println("wumpus was not killed");
                }
            }
            agent.printWorld();
        }
    }

    public static void computerGame(Agent agent) {
        while (agent.isAlive() && !agent.isGrabGold() && !agent.isGiveUp()) {
            System.out.println("powered by system, press enter to continue: ");
            Scanner scanner = new Scanner(System.in);
            if (scanner.nextLine() != null) {
                agent.simulate();
                agent.printWorld();
            }
        }
        if (agent.isAlive()) {
            agent.goBackHome();
        }
    }

    public static void testAlgorithm() {
        int times = 10000;
        int scores = 0;
        int winTimes = 0;
        int failedTimes = 0;
        int giveUpTimes = 0;

        for (int i = 0; i < times; i++) {
            // System.out.println("times of playing game: " + i);
            WumpusWorld wumpusWorld = new WumpusWorld(4, 3);
            Agent agent = new Agent(wumpusWorld);
            while (agent.isAlive() && !agent.isGrabGold() && !agent.isGiveUp()) {
                agent.simulate();
            }
            scores += agent.score;
            if (agent.isGrabGold()) {
                winTimes++;
            }
            if (!agent.isAlive()) {
                failedTimes++;
            }
            if (agent.isGiveUp()) {
                giveUpTimes++;
            }
        }
        scores /= times;
        System.out.println("this algorithm's performance is: \n"
                + "score: " + scores
                + "\nwin times: " + winTimes
                + "\nfailed times: " + failedTimes
                + "\ngive up times: " + giveUpTimes);
    }

    public static void getBestRiskRate() {
        double[] performance = {Integer.MIN_VALUE, 0, 0, 0, 0};
        int times = 10000;
        int runTimes = 0;
        //System.out.println("runTimes: "+ (++runTimes));
        for (double k = 0; k < 0.5; k += 0.005) {
            int scores = 0;
            int winTimes = 0;
            int failedTimes = 0;
            int giveUpTimes = 0;
            for (int i = 0; i < times; i++) {
                WumpusWorld wumpusWorld = new WumpusWorld(4, 3);
                Agent agent = new Agent(wumpusWorld);

                agent.setAcceptableRisk(k);
                while (agent.isAlive() && !agent.isGrabGold() && !agent.isGiveUp()) {
                    agent.simulate();
                }
                scores += agent.score;
                if (agent.isAlive()) {
                    agent.goBackHome();
                }
                if (agent.isGrabGold()) {
                    winTimes++;
                }
                if (!agent.isAlive()) {
                    failedTimes++;
                }
                if (agent.isGiveUp()) {
                    giveUpTimes++;
                }
            }

            scores /= times;
            if (scores > performance[0]) {
                performance[0] = scores;
                performance[1] = winTimes;
                performance[2] = failedTimes;
                performance[3] = giveUpTimes;
                performance[4] = k;
            }
        }

        System.out.println("this algorithm's best performance is: \n"
                + "score: " + performance[0]
                + "\nwin times: " + performance[1]
                + "\nfailed times: " + performance[2]
                + "\ngive up times: " + performance[3]
                + "\naccept risk rate: " + performance[4]);

    }
}
