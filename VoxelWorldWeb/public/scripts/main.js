
angular.module('VoxeworldApp', ['ngMaterial',"ngSanitize"])
.controller('VoxeworldMainController', function($scope, $sce, $interval, $log, $mdDialog, $mdToast, $mdSidenav) {
	var self = this;

	$scope.toggleLeft = buildToggler('left');
    $scope.toggleColor = buildToggler('colorSide');

    $scope.smoothIterations=1;

    $scope.colors = [
        {
            r : 200,
            g : 40,
            b : 40
        },
        {
            r : 40,
            g : 200,
            b : 40
        },
        {
            r : 40,
            g : 40,
            b : 200
        }
    ];
    function buildToggler(componentId) {
      return function() {
        $mdSidenav(componentId).toggle();
      };
    }

    window.onload = function() {
    	VXModule = new VXWORLD();
    };

    $scope.getPickableColorStyle = function(index) {

        style =  {
            "background-color" : 'rgba('+$scope.colors[index].r+','+$scope.colors[index].g+','+$scope.colors[index].b+', 1)'
        };
        return style;
    }

    $scope.selectColor = function(index) {
        VXModule.selectColor(
            {
                r : $scope.colors[index].r,
                g : $scope.colors[index].g,
                b : $scope.colors[index].b
            });
    }

    $scope.smoothVoxels = function()
    {
        VXModule.smoothVoxels($scope.smoothIterations);
    }

    $scope.saveVoxels = function() {
        console.log("saving");
    }

    $scope.LoadVoxels = function() {
        console.log("loading");
    }

})
.config(function($mdThemingProvider) {
  $mdThemingProvider.theme('default')
    .dark()
    .primaryPalette('teal')
    .accentPalette('light-green');
});