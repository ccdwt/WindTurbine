app.controller('Current',
   function($rootScope,$scope,$http,$location,$routeParams){
   // initialize variables    
   $scope.tabs = ['power',
                  'weather',
                  'turbine',
                  'LiveVideo'
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
     if (typeof val !== 'undefined'){
       return val>0?val:0;
     } 
     return 'NA';
   }
   $scope.safeDiv= function(a,b) {
     return b!= 0? a/b: 'INF';
   }
   $scope.hasvlc=function(){
	   for( var i = 0; i < navigator.plugins.length; i++){
		   if (navigator.plugins.item(i).name.match(/^VLC /)){
			   return true;
		   }
	   }
	   return false;
   }
   $scope.vlcpluginlink= function(){
	   if (navigator.oscpu == "Macintosh"){
		   // case Intel Mac
	   } else if (navigator.oscpu == "Mac_PowerPC"){
		   // case power PC
	   } else {
		   // default case
	   }
   }
   // start updating every 30 seconds;
   $scope.data_update();
   setInterval($scope.data_update, 30000);
   // resize handler 
   $scope.window = function(){ return window };
   window.onresize = function(){
   	$scope.vlcheight = window.innerHeight * .9;
   	$scope.vlcwidth  = window.innerWidth * .2; 
   }
   window.onresize();
   console.log ($scope);
});
