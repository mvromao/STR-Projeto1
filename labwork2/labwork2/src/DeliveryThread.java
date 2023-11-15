public class DeliveryThread extends Thread {

    private int x;
    private int z;

    public DeliveryThread(int x, int z) {
        this.x = x;
        this.z = z;
    }

    public void gotoXZ(){
        gotoXThread gotoX = new gotoXThread(x);
        gotoZThread gotoZ = new gotoZThread(z);
        gotoX.start();
        gotoZ.start();
        try{
            gotoX.join();
            gotoZ.join();
        } catch (InterruptedException e) {
        System.out.println("Thread interrupted.");
        }
    }

    public void putPart(){
        Mechanism mechanism = new Mechanism();
        mechanism.putPartInCell();
    }

    public void run() {    
        this.gotoXZ();
        this.putPart();
        System.out.println("Pallet Delivered");
    } 
}