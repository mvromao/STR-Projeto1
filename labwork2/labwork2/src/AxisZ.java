public class AxisZ implements Axis {
        
    public void moveForward() {
        Storage.moveZUp();
    }
    public void moveBackwards() {
        Storage.moveZDown();
    }

    public void stop() {
        Storage.stopZ();
    }

     public int getPos() {
        return Storage.getZPos();
    }

    public void gotoPos(int pos){  //Só para down position (1,2,3)
        int currentPos = this.getPos();
        if(currentPos == -1){
            this.moveBackwards();
            while (this.getPos() != pos){}
        }
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

    public void deliverPos() {
        int pos = this.getPos();
        if (pos == 1) {
            this.gotoPos(10);
        } else if (pos == 2) {
            this.gotoPos(20);
        } else if (pos == 3) {
            this.gotoPos(30);
        }
    }
    
    public void returnPos() {
        int pos = this.getPos();
        if (pos == 10) {
            this.gotoPos(1);
        } else if (pos == 20) {
            this.gotoPos(2);
        } else if (pos == 30) {
            this.gotoPos(3);
        }
    }

    /*
                    return 10; // position z = 1 up   
                    return 1; // position z = 1 down 
                    return 20; // position z = 2 up 
                    return 2; // position z = 2 down 
                    return 30; // position z = 3 up (cima)
                    return 3; // position z = 3 down  
     
     */
}
