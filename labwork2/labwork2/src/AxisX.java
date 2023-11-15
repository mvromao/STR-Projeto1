public class AxisX implements Axis{
    /*
     In Java, implements is a keyword used to derive one or more functionalities from interfaces. 
     Unlike class-to-class inheritance through extends, implements lets a class adhere to certain protocols, ensuring the implementing
      class offers specific functionalities, as defined in the interface
     */
    /*
     An interfaceis a completely "abstract class" that is used to group related methods with empty bodies
     */
    /*
     A class that implements a specific interface must implement the methods of the interface.
     */
    /*
     In Java, @Override is an annotation that is used to 
     indicate that a method in a subclass is intended to override a method in its superclass or interface.
     */
    //@override
    public void moveForward() {
        Storage.moveXRight();
    }

    //@override
    public void moveBackwards() {
        Storage.moveXLeft();
    }

    //@override
    public void stop() {
        Storage.stopX();
    }

    //@override
     public int getPos() {
        return Storage.getXPos();
    }
    
    // X=-1, Y=2, Z=-2
    public void gotoPos(int pos){
        int currentPos = this.getPos();
        if(currentPos == -1){
            this.moveBackwards();
            while (this.getPos() != pos){}
        }
        // if(currentPos == -1){
        //     this.moveForward();
        //     while (this.getPos() != pos){}
        // }
        if (pos < currentPos) {
            this.moveBackwards();
            while (this.getPos() != pos){}
        } 
        else if (pos > currentPos) {
            this.moveForward();
            while (this.getPos() != pos){}
        }
        this.stop();
    }
}

/*
         int currentPos = this.getPos();
        if (pos < currentPos) {
            while (this.getPos() > pos) {
                this.moveBackwards();
            }
        } 
        else if (pos > currentPos) {
            while (this.getPos() < pos) {
                this.moveForward();
            }
        }
        this.stop();
        System.out.println("Na posição desejada");
 */

    /*
                 switch(pos){
            case 1:
                while(this.getPos()!=1){
                     this.moveForward();
                }
                 this.stop();
                break;
            case 2:
                while(this.getPos()!=2){
                     this.moveForward();
                }
                this.stop();
                break;
            case 3:
                while(this.getPos()!=3){
                     this.moveForward();
                }
                this.stop();
                break;
        }
    }
     */

/*
    XXXXXXXXXXXXXXX
    if (!getBitValue(p, 2)) return 1;
    if (!getBitValue(p, 1)) return 2;
    if (!getBitValue(p, 0)) return 3;
    return(-1);

    ZZZZZZZZZZZZZZZZZZZZZZZZZ
    if (!getBitValue(p1, 3)) return 1; // position z = 1 down
    if (!getBitValue(p1, 1)) return 2; // position z = 2 down
    if (!getBitValue(p0, 7)) return 3; // position z = 3 down
    if (!getBitValue(p1, 2)) return 10; // position z = 1 up
    if (!getBitValue(p1, 0)) return 20; // position z = 2 up
    if (!getBitValue(p0, 6)) return 30; // position z = 3 up
    return(-1);

    YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
    if (!getBitValue(p0, 5)) return 1;
    if (!getBitValue(p0, 4)) return 2;
    if (!getBitValue(p0, 3)) return 3;
    return(-1);
 */
/*    public void gotoPosition(int position){
        switch(position){
            case 0:
                while(this.getPosition()!=0){   
                    this.moveBackwards();
                }
                this.stop();
                break;
            case 1:
                while(this.getPosition()!=1){
                    this.moveForward();
                }
        
                try {
                    Thread.sleep(95);
                } catch (InterruptedException ex) {
                    Logger.getLogger(Cylinder1.class.getName()).log(Level.OFF, null, ex);
                }
                this.stop();
                break;
        }
    }  */