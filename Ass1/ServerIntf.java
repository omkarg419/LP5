//Implement multi-threaded client/server Process communication using RMI.
import java.rmi.*;


interface ServerIntf extends Remote{
	// Syntax for method declaration: access_specifier return_type method_name(arguments...){ return value}
	public String stringJoin(String str1, String str2) throws RemoteException;
	
}
