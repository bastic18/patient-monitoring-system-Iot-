setInterval( update, 100 );
document.write("working");


var app = angular.module("myApp",[]);
app.controller("myCtrl",function($scope){
    $scope.name = "Hello it worked";

});




