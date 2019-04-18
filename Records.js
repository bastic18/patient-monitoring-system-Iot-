const mongoose = require('mongoose');
const Schema = mongoose.Schema;
mongoose.set('useFindAndModify', false);


//============ Create Schema =========================
var bgSchema = new mongoose.Schema({
    name:{type: String,default:"gg"},
    alert_: {
        type: String,
        default: "no",
      },
    temp:{type:Number, default:32},
    bpm:{type:Number, default:32},
    pos:{type: String, default:"No Orientation"},

});

var bastic = mongoose.model('bastic_data',bgSchema);
module.exports = bastic;
