public class Storehouse {
    private char[][] fullness;
    private String[][] palletNames;
    private String[][] referenceCodes;
    private String[][] shipmentDates;

    public Storehouse() {
        fullness = new char[3][3];
        palletNames = new String[3][3];
        referenceCodes = new String[3][3];
        shipmentDates = new String[3][3];
    }
    /*
     |02|12|22| = |13|23|33|
     |01|11|21| = |12|22|32|
     |00|10|20| = |11|21|31|
     */

    //CLEAN
    public void cleanAll() {
        for (int i = 0; i < fullness.length; i++) {
            for (int j = 0; j < fullness[i].length; j++) {
                //fullness[i][j] = '\u0000';
                fullness[i][j] = 'O';
                palletNames[i][j] = null;
                referenceCodes[i][j] = null;
                shipmentDates[i][j] = null;
            }
        }
    }
    public void cleanCell(int line, int column) {
        // Limpa uma célula específica da matriz
        //fullness[line][column] = '\u0000';
        fullness[line][column] = 'O';
        palletNames[line][column] = null;
        referenceCodes[line][column] = null;
        shipmentDates[line][column] = null;
    }

    //INSERT
    public void insertCell(int line, int column, char fullness, String palletName, String referenceCode, String shipmentDate) {
        // Insere valores em uma célula específica da matriz
        this.fullness[line][column] = fullness;
        palletNames[line][column] = palletName;
        referenceCodes[line][column] = referenceCode;
        shipmentDates[line][column] = shipmentDate;
    }

    //SHOW
    /*public void showStorehouse() {
        // Mostra todos os elementos da matriz
        for (int i = 0; i < fullness.length; i++) {
            for (int j = 0; j < fullness[i].length; j++) {
                System.out.print(fullness[i][j] + " ");
            }
            System.out.println();
        }
    } */
    public void showStorehouse() {
        // Mostra todos os elementos da matriz
        for (int j = 0; j < fullness[0].length; j++) {
            for (int i = 0; i < fullness.length; i++) {
                System.out.print(fullness[i][j] + " ");
            }
            System.out.println();
        }
    }
    public void showCell(int line, int column) {
        // Mostra um elemento específico da matriz
        //System.out.println("Fullness: " + fullness[line][column]);
        System.out.println("Pallet Name: " + palletNames[line][column]);
        System.out.println("Reference Code: " + referenceCodes[line][column]);
        System.out.println("Shipment Date: " + shipmentDates[line][column]);
    }

    public int isCellFull(int line, int column){
        if (fullness[line][column] == 'O'){
            return 0;
        } else {
            return 1;           
        }
    }
}
