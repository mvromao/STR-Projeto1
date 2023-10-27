public class  Hello_args
{
	public static void main(String[] args){
		System.out.println("-------------------");
		for (int i=0; i<args.length; i++){

			System.out.printf("\nHello arg[%d] = %s", i ,args[i]);

			System.out.printf("\nHello arg[" + i + "] = " + args[i] );

		}
				System.out.println("\n-------------------");
	}
}
