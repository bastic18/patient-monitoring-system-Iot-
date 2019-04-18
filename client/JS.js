


var app = angular.module("myApp",[]);
app.controller("myCtrl",function($scope,$http,$interval){
   

  $interval(function(){
    $http.get("/bastic/all")
    .then(function(response) {
      //$scope.name = response;
      $scope.al = response.data.alert_;
      $scope.name = response.data.temp;
      $scope.heart = response.data.bpm;
      $scope.orientation = response.data.pos;
    });


  },200);
    
    

});





