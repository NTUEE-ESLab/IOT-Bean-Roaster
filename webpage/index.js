const express = require('express');
const app = express();
 
//setting middleware
app.use(express.static(__dirname + '/html')); //Serves resources from public folder

app.use('/js', express.static(__dirname + '/js'));

const server = app.listen(5050);
