public class gotoYThread extends Thread{
    private int y;

    public gotoYThread(int y) {
        this.y = y;
    }

    public void run() {
        AxisY axisY = new AxisY();
        axisY.gotoPos(y);
    }
}
