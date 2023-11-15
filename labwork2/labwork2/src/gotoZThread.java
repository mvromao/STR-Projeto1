public class gotoZThread extends Thread{
    private int z;

    public gotoZThread(int z) {
        this.z = z;
    }

    public void run() {
        AxisZ axisZ = new AxisZ();
        axisZ.gotoPos(z);
    }
}
