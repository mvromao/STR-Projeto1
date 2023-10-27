public class MyList {  
    
    public MyList() {
        System.out.println("Object of MyList  just created�");
    }
    static public void main(String args[]) {
        java.util.LinkedList list = new java.util.LinkedList();

        list.add("aaa");    
		list.add("ccc");    
		list.add("222");    
		list.add( new Integer(100) );    
		list.add("zzz");
        
        System.out.println("------------------------");
        System.out.println( "first(list)="+list.getFirst());
        System.out.println( "last (list)="+list.getLast());
        System.out.println("------------------------");
        
        for(int i=0; i<list.size(); i++) {
            System.out.println( "list["+i+"]="+list.get(i) );
        }
        System.out.println("------------------------");
    }
}  