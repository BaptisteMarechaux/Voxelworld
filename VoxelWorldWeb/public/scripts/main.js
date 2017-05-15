
angular.module('VoxeworldApp', ['ngMaterial',"ngSanitize"])
.controller('VoxeworldMainController', function($scope, $sce, $interval, $log, $mdDialog, $mdToast, $mdSidenav) {
	var self = this;

	$scope.toggleLeft = buildToggler('left');

    function buildToggler(componentId) {
      return function() {
        $mdSidenav(componentId).toggle();
      };
    }

    VXModule = new VXWORLD();

})
.config(function($mdThemingProvider) {
  $mdThemingProvider.theme('default')
    .dark()
    .primaryPalette('teal')
    .accentPalette('light-green');
});