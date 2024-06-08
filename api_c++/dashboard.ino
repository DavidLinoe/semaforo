

void dashboard_html(){
  String content = "<html><head><title>Dashboard Dsin</title><style></style></head><body><iframe src='https://david-lino.vercel.app/'width='900px' height='600px'></iframe></body></html>";
    server.send(200, "text/html", content);

  }
