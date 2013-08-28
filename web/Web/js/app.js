'use strict';

var app = angular.module('windweb',[])
  .config(function($routeProvider) {
    $routeProvider.when('/stats/:id', {
      templateUrl: 'partials/current.html',
      controller: 'Current'
    })
    .otherwise({
      redirectTo: '/stats/power'
    });
  });


app.directive("percentMeter", function(){
  var ret = {
    restrict : 'ECA',
    scope:{
      max: '=max',
      value: '=val',
      color: '@color'
    },
    transclude: true,
    templateUrl: 'partials/Meter.html',
    controller: function($scope, $attrs,$element){
      if ($attrs.height){
        $scope.$parent.percentMeterHeight = $attrs.height;
        $scope.height = $attrs.height;
      }
      if ($attrs.width){
        $scope.$parent.percentMeterWidth= $attrs.width;
        $scope.width = $attrs.width;
      }
      if ($attrs.units){
        $scope.$parent.percentMeterUnits=$attrs.units;
        $scope.units = $attrs.units;
      }
      console.log($element);
    },
    link: function(scope,element){
      scope.Math = window.Math;
      if (scope.$parent.percentMeterHeight){
        scope.height = scope.$parent.percentMeterHeight;
      } else {
        scope.height=200;
      }
      if (scope.$parent.percentMeterWidth){
        scope.width = scope.$parent.percentMeterWidth;
      } else {
        scope.width=50;
      }
      if (scope.$parent.percentMeterUnits){
        scope.units = scope.$parent.percentMeterUnits;
      } else {
        scope.units="";
      }

    }
  };
  return ret;

});

app.directive("compassWind", function(){
  var ret = {
    restrict : 'ECA',
    scope:{
      deg: '=deg'
    },
    templateUrl: 'partials/CompassWind.html',
    link: function(scope,element){
	console.log(scope.angle);
    }
  };
  return ret;
});
