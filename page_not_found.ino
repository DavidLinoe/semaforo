  
  
  
void page_not_found_html() {
  
  String content = "<html><head><title>404 Not Found</title><style>@import url('https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap');body{font-family:Arial,sans-serif;margin:0;padding:0;display:flex;justify-content:center;align-items:center;flex-direction:column;height:100vh;background-repeat:no-repeat;background-size:cover;background-position:center;background-image:url('https://images2.imgbox.com/3b/6f/RAK7JJBl_o.jpeg')}.logo{margin:20px;background-image:url('https://images2.imgbox.com/f7/e0/UmyYPql1_o.png');background-repeat:no-repeat;position:relative;top:25px;width:220px;height:100px}.content{display:flex;flex-direction:column;justify-content:center;align-items:center;text-align:center;background-color:rgba(255,255,255,0.9);padding:20px;border-radius:15px;box-shadow:0px 0px 10px rgba(0,0,0,0.1);width:80%;max-width:600px}.content h1{font-family:'Poppins',sans-serif;font-weight:700;font-size:4em;margin:0;color:#ff6b6b}.content p{font-family:'Poppins',sans-serif;font-weight:400;font-size:1.5em;margin:10px 0;color:#333}.content a{font-family:'Poppins',sans-serif;font-weight:500;font-size:1.2em;color:#0056b3;text-decoration:none;margin-top:20px}.content a:hover{text-decoration:underline}</style></head><body><div class='content'><h1>404</h1><p>Page Not Found</p><a href='/'>Go Back to Home</a></div></body></html>";
  
  server.send(404, "text/html", content);
}
