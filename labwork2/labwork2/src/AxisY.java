public class AxisY implements Axis{
    
    public void moveForward() {
        Storage.moveYInside();
    }
    public void moveBackwards() {
        Storage.moveYOutside();
    }

    public void stop() {
        Storage.stopY();
    }

     public int getPos() {
        return Storage.getYPos();
    }

    public void gotoPos(int pos){
        int currentPos = this.getPos();
        // if(currentPos == -1){
        //     this.moveForward();
        //     while (this.getPos() != 2){}
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
