app.controller('Current',
   function($rootScope,$scope,$http,$location,$routeParams){
   // initialize variables    
   $scope.tabs = ['power',
                  'weather',
                  'turbine',
                  'historical'
                  ];
  
   // set up correct page based on routing information.
   $scope.current=$routeParams.id;
   document.title=$scope.current;
   // add scope functions
   $scope.times=function(times){
     return new Array(times);
   }

   $scope.data_update = function(){ 
     $http.get('data/data.json').then(function(res){
       console.log(res);
       $scope.data=res.data;
       $scope.Grid = res.data.TurbineExport - res.data.RowlandExport;
     });
   }
   $rootScope.tabupdate = function(tab){
      //$scope.current = $scope.tabs[tab];
      $location.path("/stats/" + $scope.tabs[tab]);
   };
   $rootScope.isActive=function(tab){
     return $scope.current == $scope.tabs[tab];
   }
   $scope.clean = function(val){
     console.log(typeof val);
     return typeof val !== 'undefined'?val:'NA';
   }
   $scope.safeDiv= function(a,b) {
     return b!= 0? a/b: 'INF';
   }
   // start updating every 30 seconds;
   $scope.data_update();
   setInterval($scope.data_update, 30000);
});
