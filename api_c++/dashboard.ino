

void dashboard_html(){
  String content = "<html><head><title>Dashboard Dsin</title><style></style></head><body><iframe src='https://7b15-2804-18-1940-ed7b-cc0d-40cc-4b2c-9744.ngrok-free.app/'width='900px' height='600px'></iframe></body></html>";
    server.send(200, "text/html", content);

  }