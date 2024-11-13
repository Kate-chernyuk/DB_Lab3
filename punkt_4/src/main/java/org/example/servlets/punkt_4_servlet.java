package org.example.servlets;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.*;

@WebServlet("")
public class punkt_4_servlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html");
        response.setCharacterEncoding("UTF-8");
        PrintWriter out = response.getWriter();

        String connectionString = "jdbc:access:/C:/DB/Lab3/pubs.mdb";

        try {
            Class.forName("com.caigen.sql.access.AccessDriver");
            Connection conn = DriverManager.getConnection(connectionString);
            Statement stmt = conn.createStatement();

            ResultSet rs = stmt.executeQuery("SELECT TOP 20 Au_id, Au_lname, Au_fname, Au_phone, Au_salary FROM Authors ORDER BY Au_id");

            out.println("<html><head><title>Таблица авторов</title>");
            out.println("<style>\n");
            out.println("body { color: blue; font-family: Arial, sans-serif; }\n");
            out.println("table { width: auto; border-collapse: collapse; background-color: gold; margin: auto; }\n");
            out.println("th, td { padding: 5px; border: 1px solid black; }\n");
            out.println("h1 { text-align: center; }\n");
            out.println("body { text-align: center; }\n");
            out.println("</style>\n");
            out.println("</head>\n");
            out.println("<body>\n");
            out.println("<h1>Таблица Авторов</h1><table border='1'><tr>\n");
            out.println("<th>Идентификатор</th><th>Фамилия</th><th>Имя</th><th>Телефон</th><th>Зарплата</th></tr>\n");

            while (rs.next()) {
                out.println("<tr><td align='center'>" + rs.getString("Au_id") + "</td><td align='left'>" + rs.getString("Au_lname") +
                        "</td><td align='left'>" + rs.getString("Au_fname") + "</td><td align='center'>" + rs.getString("Au_phone") +
                        "</td><td align='right'>" + rs.getDouble("Au_salary") + "</td></tr>");
            }

            out.println("</table></body></html>");


            rs.close();
            stmt.close();
            conn.close();
        } catch (SQLException | ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
