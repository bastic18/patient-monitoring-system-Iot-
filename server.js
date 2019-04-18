const mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/bas', {useNewUrlParser: true});
var express = require('express');
const path = require("path");
const bodyParser = require('body-parser');

var bastic = require("./Records.js");


var app = express();
app.use(express.static(path.join(__dirname,"./client")));
var fs= require("fs");


/* Middlewares */

app.use(bodyParser.urlencoded({ extended: false }));


app.post('/write-file', function (req, res) {
    const body = req.body; // your request body
    bastic.findById({_id:"5c95391cd64241499c26ab3b"},function(err,docs){    res.json(docs);});
    // your "magical" code
    
});



//=========== Create Model for Schema =================
//var rpm = mongoose.model('rpm',schema);

//=========== Send to Database ========================
var first = new bastic ({
    name:"gg",
    alert: "no",
    temp:32,
    bpm:67,
    pos:"On Back",
});

//first.save();
//=======================================================
/*
function landing(req, res)
{
    app.send("/mongodb1/client");
}*/

app.get("/", function(req,res){
    res.sendFile('index.html',{root: path.join(__dirname,"./client")})

} );



//==================Retreive from Database ===============
//rpmRecords.findById({_id:"5c9290d33acbc60120ee0e9e"},function(err,docs){    console.log(docs);docs.bpm = 16; docs.save(); });   //----> find by ID  then update// type it as a string


function espPost(req, res)  //make them write most of this
{
  console.log(req.body);
  var successMessage = {
    success:true
  };
  //fs.writeFileSync("patient.md", `PATIENT LOG FROM DATABASE \n ================================\n\n`);
  // console.log(req.body.id) ;
  bastic.findOne({name:"gg"},function(err,docs){    res.json(docs); docs.alert_ = req.body.alert_; docs.temp = req.body.temp; docs.bpm = req.body.bpm;   docs.pos = req.body.pos; docs.save(); }); //}); )
    //.then(r => res.send("Message received"));
   // var myData = new bastic (req.body);
  //  myData.save()
  var word= req.body
  var data= JSON.stringify(word);
  setInterval(function log(){
fs.appendFile("patient.md", `* alert =${req.body.alert_}   temperature = ${req.body.temp}    heart rate= ${req.body.bpm}    position=${req.body.pos} \n`, (err)=>{
  if (err) throw err;
  console.log("file updated");
});
  }, 30000);

  setInterval(function log(){
    fs.appendFile("log.csv", `${req.body.temp},${req.body.bpm}\n`, (err)=>{
      if (err) throw err;
      console.log("file updated");
    });
      }, 30000);

}



//========================================================

//   
app.get('/bas', function(req, res){
    bastic.find(function(err,docs){    res.json(docs);}); //res.send('new hello world');
  });

app.get('/bastic/all', function(req, res){
    bastic.findById({_id:"5c959f1bb5e7fc28b82b57c6"},function(err,docs){   res.json(docs);}); //res.send('new hello world');
  });  
  
 
  app.get('/update' , function(req,res){ res.send(req.query ); console.log(req.query.call) ;console.log(req.query.but) ;});      // rpm.findOne({name:"real"},function(err,docs){    res.json(docs); docs.temperature = 216500; docs.save(); }); });
  
  app.post("/espPost",espPost);

  

  app.listen(1020);
