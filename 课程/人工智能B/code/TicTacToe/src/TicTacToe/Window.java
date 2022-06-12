package TicTacToe;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;

import Algorithm.*;

/**
 * @author null
 */
public class Window extends JFrame {

    private static final int WIDTH = 600;
    private static final int HEIGHT = 600;

    private final Board board;
    private final Panel panel;
    private BufferedImage imageBackground, imageX, imageO, imageIcon;


    public static void main(String[] args) {
        SwingUtilities.invokeLater(Window::new);
    }

    private enum Mode {
        /**
         * player game Mode, two players take steps manually
         */
        MAN_MAN_BATTLE,
        /**
         * computer game Mode, one player takes steps automatically
         */
        MAN_AI_BATTLE,
        /**
         * computer game Mode, two players take steps automatically
         */
        AI_AI_BATTLE
    }

    private static Mode mode;

    /**
     * The center location of each of the cells is stored here.
     * Used for identifying which cell the player has clicked on.
     */
    private Point[] cells;

    /**
     * The distance away from the center of a cell that will register
     * as a click.
     */
    private static final int DISTANCE = 100;

    /**
     * Construct the Window.
     */
    private Window() {
        loadImages();
        board = new Board();
        loadCells();
        panel = createPanel();
        createMenu();
        setWindowProperties();
    }

    /**
     * Load the locations of the center of each of the cells.
     */
    private void loadCells() {
        cells = new Point[9];

        cells[0] = new Point(109, 109);
        cells[1] = new Point(299, 109);
        cells[2] = new Point(489, 109);
        cells[3] = new Point(109, 299);
        cells[4] = new Point(299, 299);
        cells[5] = new Point(489, 299);
        cells[6] = new Point(109, 489);
        cells[7] = new Point(299, 489);
        cells[8] = new Point(489, 489);
    }

    /**
     * Set the size, title, visibility etc...
     */
    private void setWindowProperties() {
        setResizable(false);
        pack();
        setTitle("Tic Tac Toe");
        setIconImage(imageIcon);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);
    }

    /**
     * Create the panel that will be used for drawing Tic Tac Toe to the screen.
     *
     * @return the panel with the specified dimensions and mouse listener
     */
    private Panel createPanel() {
        Panel panel = new Panel();
        Container contentPane = getContentPane();
        contentPane.add(panel);
        panel.setPreferredSize(new Dimension(WIDTH, HEIGHT));
        panel.addMouseListener(new MyMouseAdapter());
        return panel;
    }

    /**
     *
     */
    private void createMenu() {
        JMenu menu = new JMenu("GameMode");

        JMenuItem item1 = new JMenuItem(new AbstractAction("Man-Man") {
            @Override
            public void actionPerformed(ActionEvent e) {
                mode = Mode.MAN_MAN_BATTLE;
            }
        });
        JMenuItem item2 = new JMenuItem(new AbstractAction("Man-AI") {
            @Override
            public void actionPerformed(ActionEvent e) {
                mode = Mode.MAN_AI_BATTLE;
                if (Math.random() >= 0.5) {
                    Algorithms.alphaBetaAdvanced(board);
                    panel.repaint();
                }
            }
        });
        JMenuItem item3 = new JMenuItem(new AbstractAction("AI-AI") {
            @Override
            public void actionPerformed(ActionEvent e) {
                mode = Mode.AI_AI_BATTLE;
                computersGame();
            }
        });
        menu.add(item1);
        menu.add(item2);
        menu.add(item3);

        JMenuBar menuBar = new JMenuBar();
        menuBar.add(menu);
        this.setJMenuBar(menuBar);
    }

    /**
     * Load the image of the background and the images of the X and O
     */
    private void loadImages() {
        imageBackground = getImage("background.png");
        imageX = getImage("x.png");
        imageO = getImage("o.png");
        imageIcon = getImage("icon.png");
    }

    /**
     * Helper method for grabbing the images from the disk.
     *
     * @param imageName the name of the image
     * @return the image that was grabbed
     */
    private static BufferedImage getImage(String imageName) {

        BufferedImage image;

        try {
            URL path = Window.class.getResource( "/Source/" + imageName);
            System.out.println(path);
            image = ImageIO.read(path);
        } catch (IOException ex) {
            throw new RuntimeException("Image could not be loaded.");
        }

        return image;
    }

    /**
     * Used for drawing Tic Tac Toe to the screen.
     */
    private class Panel extends JPanel {

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            paintTicTacToe((Graphics2D) g);
        }

        /**
         * The main painting method that paints everything.
         *
         * @param g the Graphics object that will perform the panting
         */
        private void paintTicTacToe(Graphics2D g) {
            setProperties(g);
            paintBoard(g);
            paintWinner(g);
        }

        /**
         * Set the rendering hints of the Graphics object.
         *
         * @param g the Graphics object to set the rendering hints on
         */
        private void setProperties(Graphics2D g) {
            g.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
                    RenderingHints.VALUE_INTERPOLATION_BILINEAR);
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                    RenderingHints.VALUE_ANTIALIAS_ON);
            g.drawImage(imageBackground, 0, 0, null);

            // The first time a string is drawn it tends to lag.
            // Drawing something trivial at the beginning loads the font up.
            // Better to lag at the beginning than during the middle of the game.
            g.drawString("", 0, 0);
        }

        /**
         * Paints the background image and the X's and O's.
         *
         * @param g the Graphics object that will perform the panting
         */
        private void paintBoard(Graphics2D g) {
            Board.State[][] boardArray = board.toArray();
            int boardGrid = 3;
            int offset = 20;

            for (int y = 0; y < boardGrid; y++) {
                for (int x = 0; x < boardGrid; x++) {
                    if (boardArray[y][x] == Board.State.X) {
                        g.drawImage(imageX, offset + 190 * x, offset + 190 * y, null);
                    } else if (boardArray[y][x] == Board.State.O) {
                        g.drawImage(imageO, offset + 190 * x, offset + 190 * y, null);
                    }
                }
            }
        }

        /**
         * Paints who won to the screen.
         *
         * @param g the Graphics object that will perform the panting
         */
        private void paintWinner(Graphics2D g) {
            if (board.isGameOver()) {
                g.setColor(new Color(240, 0, 86));
                g.setFont(new Font("TimesRoman", Font.PLAIN, 60));

                String s;

                if (board.getWinner() == Board.State.Blank) {
                    s = "Draw";
                } else {
                    s = board.getWinner() + " Wins!";
                }

                g.drawString(s, 300 - getFontMetrics(g.getFont()).stringWidth(s) / 2, 315);

            }
        }
    }

    /**
     * AI to AI Battle
     */
    private void computersGame() {
        Timer timer = new Timer(500, null);
        timer.addActionListener((e) -> {
            if (!board.isGameOver()) {
                Algorithms.alphaBetaAdvanced(board);
                panel.repaint();
            } else {
                timer.stop();
            }
        });
        timer.start();
    }

    /**
     * For detecting mouse clicks.
     */
    private class MyMouseAdapter extends MouseAdapter {
        @Override
        public void mousePressed(MouseEvent e) {
            super.mouseClicked(e);

            if (board.isGameOver()) {
                board.reset();
                panel.repaint();
            } else {
                playMove(e);
            }
        }

        /**
         * Plays the move that the user clicks, if the move is valid.
         *
         * @param e the MouseEvent that the user performed
         */
        private void playMove(MouseEvent e) {
            int moveIndex = transClickToIndex(e.getPoint());

            if (!board.isGameOver() && moveIndex != -1) {
                boolean validMove = board.move(moveIndex);
                if (mode == Mode.MAN_AI_BATTLE && validMove && !board.isGameOver()) {
                    Algorithms.alphaBetaAdvanced(board);
                }
                panel.repaint();
            }
        }

        /**
         * Translate the mouse click position to an index on the board.
         *
         * @param clickPoint the location of where the player pressed the mouse
         * @return the index on the Tic Tac Toe board (-1 if invalid click)
         */
        private int transClickToIndex(Point clickPoint) {
            for (int i = 0; i < cells.length; i++) {
                if (distance(cells[i], clickPoint) <= DISTANCE) {
                    return i;
                }
            }
            return -1;
        }

        /**
         * Distance between two points.
         * Used for determining if the player has pressed on a cell to play a move.
         *
         * @param p1 the first point (intended to be the location of the cell)
         * @param p2 the second point (intended to be the location of the mouse click)
         * @return the distance between the two points
         */
        private double distance(Point p1, Point p2) {
            return Math.sqrt(Math.pow(p1.getX() - p2.getX(), 2) + Math.pow(p1.getY() - p2.getY(), 2));
        }
    }
}
