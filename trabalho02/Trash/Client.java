/*
 * O cliente deve executar as operações com as matrizes e salvar os dados (recuperar e por fim excluir o arquivo)
 */
import RMI.IDatabase;
import RMI.IMatrix;

public class Client {
    public static void main(String[] args)
    {
        String host_db_API = args[0];
        String host_matrix_API = args[1];
        try
        {
                Registry reg_db = LocateRegistry.getRegistry(host_db_API);
                Registry reg_matrix = LocateRegistry.getRegistry(host_matrix_API);

                IDatabase db_stub = (IDatabase) registryDB.lookup("db-server");
                IMatrix matrix_stub = (IMatrix) registryMatrix.lookup("matrix-server");

        }
    }
}
