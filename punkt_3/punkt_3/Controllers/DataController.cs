using Microsoft.AspNetCore.Mvc;
using System.Data;
using System.Data.OleDb;

namespace punkt2.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class DataController : Controller
    {
        private List<object> GetAuthorsData()
        {
            string connectionString = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\DB\\Lab3\\pubs.mdb";
            List<object> data = new List<object>();

            using (OleDbConnection connection = new OleDbConnection(connectionString))
            {
                connection.Open();
                using (OleDbCommand command = new OleDbCommand("SELECT TOP 20 * FROM Authors", connection))
                {
                    using (OleDbDataReader reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            data.Add(new
                            {
                                Au_id = reader["Au_id"],
                                Au_lname = reader["Au_lname"],
                                Au_fname = reader["Au_fname"],
                                Au_phone = reader["Au_phone"],
                                Au_salary = reader["Au_salary"]
                            });
                        }
                    }
                }
            }
            return data;
        }

        [HttpGet]
        public IActionResult Index()
        {
            return View("DataDisplay", GetAuthorsData());
        }

    }
}
