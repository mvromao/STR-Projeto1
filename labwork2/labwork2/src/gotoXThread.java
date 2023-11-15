public class gotoXThread extends Thread{
    private int x;

    public gotoXThread(int x) {
        this.x = x;
    }

    public void run() {
        AxisX axisX = new AxisX();
        axisX.gotoPos(x);
    }
}
