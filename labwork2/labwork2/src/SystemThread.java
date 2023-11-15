import java.util.Scanner;

public class SystemThread extends Thread{

    public void run(){
        Scanner scanner = new Scanner(System.in);
        int input;

        AxisX axisX = new AxisX(); //para retirar
        AxisY axisY = new AxisY(); //para retirar
        AxisZ axisZ = new AxisZ(); //para retirar

        Mechanism mechanism = new Mechanism();
        Storehouse storehouse = new Storehouse();
        storehouse.cleanAll();

        //autoCalibrate();
        calibrate();

        while(true){
            showMenu();
            System.out.println("\u001B[32mEnter a command:\u001B[0m");
            System.out.println(">");

            input = scanner.nextInt();
            if (input == 0 || input == 'x') {
                System.out.println("Exiting program...");
                scanner.close();
                System.exit(0);
                //break;
            }
            processInput(input, axisX, axisY, axisZ, mechanism, storehouse);
        }
    } 

    public void processInput(int input, AxisX axisX, AxisY axisY, AxisZ axisZ, Mechanism mechanism, Storehouse storehouse){
        switch(input){
            case 1:
                Scanner pos = new Scanner(System.in);
                System.out.println("X=");
                int c = pos.nextInt();
                System.out.println("Z=");
                int v = pos.nextInt();
                if (c >= 1 && c <= 3 && v >= 1 && v <= 3) {
                    if (storehouse.isCellFull(c-1, v-1)!=1){
                        pos.nextLine(); // Limpa o buffer do scanner
                        System.out.print("Insira o pallet Name: ");
                        String palletName = pos.nextLine();
                        System.out.print("Insira o código de referência: ");
                        String referenceCode = pos.nextLine();
                        System.out.print("Insira a data de envio: ");
                        String shipmentDate = pos.nextLine();
                        storehouse.insertCell(c-1, v-1, 'X',palletName, referenceCode, shipmentDate);
                        DeliveryThread deliveryThread = new DeliveryThread(c, v);

                        deliveryThread.start(); 
                        try {
                            deliveryThread.join();
                        } catch (InterruptedException e) {
                            System.out.println("Thread interrupted.");
                        }
                    } else{
                        System.out.println("Position occupied!");
                    }
                } else {
                    System.out.println("Posição inválida.");
                }
                break;
            case 2: 
                autoCalibrate();
                break;
            case 3:
                Scanner scanner = new Scanner(System.in);
                char po;
                do {
                    System.out.println("Fullness:");
                    storehouse.showStorehouse();
                    System.out.println("Enter 'x' to exit:");
                    po = scanner.next().charAt(0);
                } while (po != 'x');
                break;
            case 4:
                //axisZ.deliverPos();
                mechanism.putPartInCell();
                break;
            case 5:
                mechanism.takePartFromCell();
                break;
            case 6:
                break;
            case 7: 
                System.out.println("Para 1, 2 ou 3?");
                Scanner j = new Scanner(System.in);
                String input7 = j.nextLine();
                char m = input7.charAt(0);
                int i = Character.getNumericValue(m);
                axisX.gotoPos(i);
                break;
            case 8: 
                System.out.println("Para 1, 2 ou 3?");
                Scanner h = new Scanner(System.in);
                String input8 = h.nextLine();
                char x = input8.charAt(0);
                int z = Character.getNumericValue(x);
                axisY.gotoPos(z);
                break;
            case 9: 
                System.out.println("Para 1 a 3 (Só down)?");
                Scanner k = new Scanner(System.in);
                String input9 = k.nextLine();
                char p = input9.charAt(0);
                int l = Character.getNumericValue(p);
                axisZ.gotoPos(l);
                break;
        }
    }

    static public void calibrate(){
        System.out.println("\u001B[33mYou need to calibrate the System\u001B[0m"); 

        Axis axisX = new AxisX();
        Axis axisY = new AxisY();
        Axis axisZ = new AxisZ();

        Scanner scanner = new Scanner(System.in);
        // CalibrationThread threadX = new CalibrationThread(axisX);
        // CalibrationThread threadY = new CalibrationThread(axisY);
        // CalibrationThread threadZ = new CalibrationThread(axisZ);
        boolean Xcalib = false;
        boolean Ycalib = false;
        boolean Zcalib = false;

        int l = axisX.getPos();
        int m = axisZ.getPos();
        int o = axisY.getPos();
        System.out.println(l);
        System.out.println(m);
        System.out.println(o);

        while(!Xcalib || !Ycalib || !Zcalib){
            calibrationMenu();
            System.out.println(">");
            String inputc = scanner.next();
            char c = inputc.charAt(0);
            //char inputc = scanner.nextChar();
            switch(c){
                case 'w': //Z(^)
                    if (Ycalib) {
                        int z = axisZ.getPos();
                        if(z==-1){z=2;}
                        if( z== 3 ){
                            System.out.println("Invalid movement!");
                        } else{
                            z++;
                            gotoZThread thread = new gotoZThread(z);
                            thread.start();
                            try{
                                thread.join();
                            } catch (InterruptedException e) {
                                System.out.println("Thread interrupted.");
                            }
                        }
                        if(axisZ.getPos() == 1){
                            System.out.println("Z calibrated");
                            Zcalib = true;
                        } else{  
                            System.out.println("Z uncalibrated");
                            Zcalib = false;
                        }
                    } else {System.out.println("Y must be done first.");}
                    break;
                case 's': //Z(v)
                    if (Ycalib) {
                        int z = axisZ.getPos();
                        if(z==-1){z=2;}
                        if( z == 0 ){
                            System.out.println("Invalid movement!");
                        } else{
                            z--;
                            gotoZThread thread = new gotoZThread(z);
                            thread.start();
                            try{
                                thread.join();
                            } catch (InterruptedException e) {
                                System.out.println("Thread interrupted.");
                            }
                        }
                        if(axisZ.getPos() == 1){
                            System.out.println("Z calibrated");
                            Zcalib = true;
                        } else{  
                            System.out.println("Z uncalibrated");
                            Zcalib = false;
                        }
                    } else {System.out.println("Y must be done first.");}
                    break;
                case 'a': //X(<)
                    if (Ycalib) {
                        int x = axisX.getPos();
                        if(x==-1){x=2;}
                        if( x == 0 ){
                            System.out.println("Invalid movement!");
                        } else{
                            x--;
                            gotoXThread thread = new gotoXThread(x);
                            thread.start();
                            try{
                                thread.join();
                            } catch (InterruptedException e) {
                                System.out.println("Thread interrupted.");
                            }
                        }
                        if(axisX.getPos() == 1){
                            System.out.println("X calibrated");
                            Xcalib = true;
                        } else{  
                            System.out.println("X uncalibrated");
                            Xcalib = false;
                        }
                    } else {System.out.println("Y must be done first.");}
                    break;
                case 'd': //X(>)
                    if (Ycalib) {
                        int x = axisX.getPos();
                        if(x==-1){x=2;}
                        if( x == 3 ){
                            System.out.println("Invalid movement!");
                        } else{
                            x++;
                            gotoXThread thread = new gotoXThread(x);
                            thread.start();
                            try{
                                thread.join();
                            } catch (InterruptedException e) {
                                System.out.println("Thread interrupted.");
                            }
                        }
                        if(axisX.getPos() == 1){
                            System.out.println("X calibrated");
                            Xcalib = true;
                        } else{  
                            System.out.println("X uncalibrated");
                            Xcalib = false;
                        }
                    } else {System.out.println("Y must be done first.");}
                    break;
                case 'e': //Y(IN)
                {
                    int y = axisY.getPos();
                    if(y==-1){y=2;}
                    if( y == 3 ){
                        System.out.println("Invalid movement!");
                    } else{
                        y++;
                        gotoYThread thread = new gotoYThread(y);
                        thread.start();
                        try{
                            thread.join();
                        } catch (InterruptedException e) {
                            System.out.println("Thread interrupted.");
                        }
                    }
                    if(axisY.getPos() == 2){
                        System.out.println("Y calibrated");
                        Ycalib = true;
                    } else{  
                        System.out.println("Y uncalibrated");
                        Ycalib = false;
                    }
                    break;
                }
                case 'q': //Y(OUT)
                    int y = axisY.getPos();
                    if( y == -1 ){ y = 2; } 
                    if( y == 0 ){
                        System.out.println("Invalid movement!");
                    } else{
                        y--;
                        gotoYThread thread = new gotoYThread(y);
                        thread.start();
                        try{
                            thread.join();
                        } catch (InterruptedException e) {
                            System.out.println("Thread interrupted.");
                        }
                    }
                    if(axisY.getPos() == 2){
                        System.out.println("Y calibrated");
                        Ycalib = true;
                    } else{  
                        System.out.println("Y uncalibrated");
                        Ycalib = false;
                    }
                    break;
                default:
                    System.out.println("Invalid command.");
                    break;
            }
        }
        //     if (Xcalib && Ycalib && Zcalib) {
  
        //         break;
        //     }
        // }
        System.out.println("Everything calibrated");
        scanner.close();
    }

    static public void autoCalibrate(){

        System.out.println("\nCalibrating... \n\n");

        Axis axisX = new AxisX();
        Axis axisY = new AxisY();
        Axis axisZ = new AxisZ();

        CalibrationThread threadY = new CalibrationThread(axisY);
        threadY.start();
        try{
            threadY.join();
        } catch (InterruptedException e) {
        System.out.println("Thread interrupted.");
        }
        CalibrationThread threadX = new CalibrationThread(axisX);
        CalibrationThread threadZ = new CalibrationThread(axisZ);
        threadX.start();
        threadZ.start();
        try {
            threadX.join();
            threadZ.join();
        } catch (InterruptedException e) {
            System.out.println("Thread interrupted.");
        }
        System.out.println("Calibrated \n\n");
    }

    static private void showMenu() {
        //System.out.println("\u001B[36mPlease select an option:\u001B[0m");
        System.out.println("\u001B[32m(1) - Store a pallet in a specific x, z position\u001B[0m");
        System.out.println("\u001B[33m(2) - Recalibrate\u001B[0m");     
        System.out.println("\u001B[35m(3) - Show Storehouse\u001B[0m");
        System.out.println("\u001B[31m(4) - TESTE1\u001B[0m");
        System.out.println("\u001B[31m(5) - TESTE2\u001B[0m");
        System.out.println("\u001B[31m(6) - TESTE3\u001B[0m");
        System.out.println("\u001B[31m(7) - gotoX\u001B[0m");
        System.out.println("\u001B[31m(8) - gotoY\u001B[0m");
        System.out.println("\u001B[31m(9) - gotoZ\u001B[0m");
        System.out.println("\u001B[34m(0) - Exit the program\u001B[0m");
    }
    static private void calibrationMenu() {
        System.out.println("\u001B[35mCalibration keys:\u001B[0m");
        System.out.println("\u001B[31m(q)(OUT)-(w)(^)-(e)(IN )\u001B[0m");
        System.out.println("\u001B[31m(a)( < )-(s)(v)-(d)( > )\u001B[0m");
        // System.out.println("\u001B[31m(w)(^)\u001B[0m");
        // System.out.println("\u001B[31m(s)(v)\u001B[0m");
        // System.out.println("\u001B[31m(a)(<)\u001B[0m");
        // System.out.println("\u001B[31m(d)(>)\u001B[0m");
        // System.out.println("\u001B[31m(e)(IN)\u001B[0m");
        // System.out.println("\u001B[31m(q)(OUT)\u001B[0m");
    }
}
/*
 \u001B[31m: Red
\u001B[32m: Green
\u001B[33m: Yellow
\u001B[34m: Blue
\u001B[35m: Purple
\u001B[36m: Cyan
 */
