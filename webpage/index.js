const express = require('express');
const app = express();
const mysql = require('mysql')
 
//setting middleware
app.use(express.static(__dirname + '/html')); //Serves resources from public folder

app.use('/js', express.static(__dirname + '/js'));
app.use('/img', express.static(__dirname + '/img'));
app.use('/nanogallery_dist', express.static(__dirname + '/nanogallery_dist'));

app.use(express.json());

const server = app.listen(5050);

const con = mysql.createConnection({
    host: "140.112.174.222",
    user: "mks_website",
    password: "ntueesaad",
    database : 'db_mks_website'
});

con.connect(function(err) {
    if (err) throw err;
    console.log("Mysql Connected!");
    // makeRecord();
});

app.post('/saveData', function(request, response){
    
    let sql_ins = `INSERT INTO tbl_coffee (label) VALUES ('${request.body.profile_name}');`;
    con.query(sql_ins, function (err, result) {
        if (err) throw err;
        console.log("tbl_coffee 1 record inserted");
        console.log(result.insertId);
        let record_id = result.insertId;
        let values = [];
        for (let i=0;i<request.body.temperature.length; ++i)
        {
            // sql_insert_str += `INSERT INTO tbl_coffee_record (record_id,temperature,roast,humidity) VALUES (${record_id},${request.body.temperature[i]},${request.body.roast[i]},${request.body.humidity[i]}) ;`;
            values.push([record_id,request.body.temperature[i],request.body.roast[i],request.body.humidity[i]])
        }

        let sql = `INSERT INTO tbl_coffee_record (record_id,temperature,roast,humidity) VALUES ?`;
        
    

        con.query(sql, [values],function(err, rows, fields){
            if (err) throw err;
            console.log("OK")
            
        });
        response.json(request.body);    // echo the result back
    });
        
    
      
      
  });

app.get('/getAllData', function(req, res) {
    let data = {
        "Data" : ""
    };
    con.query("SELECT * from tbl_coffee ORDER BY `idtbl_coffee` DESC",function(err, rows, fields){
        if(rows.length != 0){
            
            data["Data"] = rows;
            // console.log(rows);
            res.json(data);
        }else{
            data["Data"] = 'No data Found..';
            res.json(data);
        }
    });

});

app.get('/getAllRecordData', function(req, res) {
    let data = {
        "Data" : ""
    };
    con.query(`SELECT * from tbl_coffee_record WHERE record_id = ${req.query.id}`,function(err, rows, fields){
        if(rows.length != 0){
            
            data["Data"] = rows;
            // console.log(rows);
            res.json(data);
        }else{
            data["Data"] = 'No data Found..';
            res.json(data);
        }
    });

});





// app.get('/getGraph', function() )

function makeRecord()
{
    let label_content = "asd"
    let sql = `INSERT INTO tbl_coffee (label) VALUES ('${label_content}');`;
    con.query(sql, function (err, result) {
        if (err) throw err;
        console.log("tbl_coffee 1 record inserted");
    });
}

function getRecord(id)
{
    let data = {
        "Data" : ""
    };
    con.query(`SELECT * from tbl_coffee_record WHERE record_id = ${id}`,function(err, rows, fields){
        if(rows.length != 0){
            
            data["Data"] = rows;
            console.log(rows);
            // res.json(data);
        }else{
            data["Data"] = 'No data Found..';
            // res.json(data);
        }
    });
}


getAllRecord();

function getAllRecord()
{
    let data = {
        "Data" : ""
    };
    con.query("SELECT * from tbl_coffee",function(err, rows, fields){
        if(rows.length != 0){
            
            data["Data"] = rows;
            console.log(rows);
            // res.json(data);
        }else{
            data["Data"] = 'No data Found..';
            // res.json(data);
        }
    });
}