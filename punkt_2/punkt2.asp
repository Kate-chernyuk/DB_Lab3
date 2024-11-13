<%
On Error Resume Next

Dim conn, rs, sql
Set conn = Server.CreateObject("ADODB.Connection")
Set rs = Server.CreateObject("ADODB.Recordset")

conn.ConnectionString = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\DB\Lab3\pubs.mdb;"
conn.Open
sql = "SELECT TOP 20 Au_id, Au_lname, Au_fname, Au_phone, Au_salary FROM Authors ORDER BY Au_id"

If Err.Number <> 0 Then
    Response.Write("Ошибка подключения к базе данных: " & Err.Description)
    Response.End
End If

rs.Open sql, conn

Response.Write "<html><head><title>Authors</title>"
Response.Write "<meta charset='utf-8' />" 
Response.Write "<style>"
Response.Write "body { color: blue; font-family: Arial, sans-serif; }"
Response.Write "table { width: auto; border-collapse: collapse; background-color: gold; margin: auto; }"
Response.Write "th, td { padding: 5px; border: 1px solid black; }"
Response.Write "h1 { text-align: center; }"
Response.Write "</style></head><body { text-align: center; }>"
Response.Write "<h1>Таблица Авторов</h1><table border='1'><tr>"
Response.Write "<th>Идентификатор</th><th>Фамилия</th><th>Имя</th><th>Телефон</th><th>Зарплата</th></tr>"
Do While Not rs.EOF
    Response.Write "<tr>"
    Response.Write "<td align='center'>" & rs("Au_id") & "</td>"
    Response.Write "<td align='left'>" & rs("Au_lname") & "</td>"
    Response.Write "<td align='left'>" & rs("Au_fname") & "</td>"
    Response.Write "<td align='center'>" & rs("Au_phone") & "</td>"
    Response.Write "<td align='right'>" & rs("Au_salary") & "</td>"
    Response.Write "</tr>"
    rs.MoveNext
Loop
Response.Write "</table></body></html>"

rs.Close
conn.Close
Set rs = Nothing
Set conn = Nothing
%>