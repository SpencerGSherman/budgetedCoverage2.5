minimumPurchaseData = fopen('minimumPurchaseData.txt','r');
minimumSensors = fscanf(minimumPurchaseData,'(%d, %d)');
fclose(minimumPurchaseData);
randomPurchaseData = fopen('randomPurchaseData.txt','r');
randomSensors = fscanf(randomPurchaseData,'(%d, %d)');
fclose(randomPurchaseData);
optimizedPurchaseData = fopen('optimizedPurchaseData.txt','r');
optimizedSensors = fscanf(optimizedPurchaseData,'(%d, %d)');
fclose(optimizedPurchaseData);

citySensorsX = randomSensors(1:2:200);
citySensorsY = randomSensors(2:2:200);

minimumPurchasesX = minimumSensors(201:2:end);
minimumPurchasesY = minimumSensors(202:2:end);
randomPurchasesX = randomSensors(201:2:end);
randomPurchasesY = randomSensors(202:2:end);
optimizedPurchasesX = optimizedSensors(201:2:end);
optimizedPurchasesY = optimizedSensors(202:2:end);

scatter(citySensorsX,citySensorsY,'x','blue');
hold on
centers = [minimumPurchasesX minimumPurchasesY];
viscircles(centers,5,'LineStyle','--', 'LineWidth',1,'Color',"cyan");
centers = [randomPurchasesX randomPurchasesY];
viscircles(centers,5,'LineStyle','--', 'LineWidth',1,'Color',"green");
centers = [optimizedPurchasesX optimizedPurchasesY];
viscircles(centers,5,'LineStyle','--', 'LineWidth',1,'Color',"red");
h = zeros(4, 1);
h(1) = plot(NaN,NaN,'o','Color', "green");
h(2) = plot(NaN,NaN,'o','Color',"cyan");
h(3) = plot(NaN,NaN,'o','Color',"red");
h(4) = plot(NaN,NaN,'x','Color',"blue");
legend(h, 'Random','Greedy','Optimized','Sensors');
xlabel('x Position');
ylabel('y Position');
title('City Purchase Map');
grid on
pbaspect([1 1 1])

xlim([0 100]);
ylim([0 100]);

figure;
LakelandPerformanceCompare = fopen('LakelandCoveragePerformance.txt','r');
performancePointsLakeland = fscanf(LakelandPerformanceCompare,'(%d, %d)');
fclose(LakelandPerformanceCompare);
MiamiPerformanceCompare = fopen('MiamiCoveragePerformance.txt','r');
performancePointsMiami = fscanf(MiamiPerformanceCompare,'(%d, %d)');
fclose(MiamiPerformanceCompare);
BostonPerformanceCompare = fopen('BostonCoveragePerformance.txt','r');
performancePointsBoston = fscanf(BostonPerformanceCompare,'(%d, %d)');
fclose(BostonPerformanceCompare);
DesMoinesPerformanceCompare = fopen('DesMoinesCoveragePerformance.txt','r');
performancePointsDesMoines = fscanf(DesMoinesPerformanceCompare,'(%d, %d)');
fclose(DesMoinesPerformanceCompare);
LuisVillePerformanceCompare = fopen('LuisvilleCoveragePerformance.txt','r');
performancePointsLuisVille = fscanf(LuisVillePerformanceCompare,'(%d, %d)');
fclose(LuisVillePerformanceCompare);

budgetLakeland = performancePointsLakeland(1:6:end);
randomLakeland = performancePointsLakeland(2:6:end);
greedyLakeland = performancePointsLakeland(4:6:end);
optimizedLakeland = performancePointsLakeland(6:6:end);
budgetMiami = performancePointsMiami(1:6:end);
randomMiami = performancePointsMiami(2:6:end);
greedyMiami = performancePointsMiami(4:6:end);
optimizedMiami = performancePointsMiami(6:6:end);
budgetBoston = performancePointsBoston(1:6:end);
randomBoston = performancePointsBoston(2:6:end);
greedyBoston = performancePointsBoston(4:6:end);
optimizedBoston = performancePointsBoston(6:6:end);
budgetDesMoines = performancePointsDesMoines(1:6:end);
randomDesMoines = performancePointsDesMoines(2:6:end);
greedyDesMoines = performancePointsDesMoines(4:6:end);
optimizedDesMoines = performancePointsDesMoines(6:6:end);
budgetLuisVille = performancePointsLuisVille(1:6:end);
randomLuisVille = performancePointsLuisVille(2:6:end);
greedyLuisVille = performancePointsLuisVille(4:6:end);
optimizedLuisVille = performancePointsLuisVille(6:6:end);

tg = uitabgroup();

t1 = uitab(tg);
ax1 = axes('Parent', t1);
plot(budgetLakeland,randomLakeland,'Color',"green");
hold on
plot(budgetLakeland,greedyLakeland,'Color',"cyan");
plot(budgetLakeland,optimizedLakeland,'Color',"red");
xlabel('Budget');
ylabel('Coverage');
legend('Random','Greedy','Optimized');
grid on
title(ax1, 'Lakeland');

t2 = uitab(tg);
ax2 = axes('Parent', t2);
plot(budgetMiami,randomMiami,'Color',"green");
hold on
plot(budgetMiami,greedyMiami,'Color',"cyan");
plot(budgetMiami,optimizedMiami,'Color',"red");
xlabel('Budget');
ylabel('Coverage');
legend('Random','Greedy','Optimized');
grid on
title(ax2, 'Miami');

t3 = uitab(tg);
ax3 = axes('Parent', t3);
plot(budgetBoston,randomBoston,'Color',"green");
hold on
plot(budgetBoston,greedyBoston,'Color',"cyan");
plot(budgetBoston,optimizedBoston,'Color',"red");
xlabel('Budget');
ylabel('Coverage');
legend('Random','Greedy','Optimized');
grid on
title(ax3, 'Boston');

t4 = uitab(tg);
ax4 = axes('Parent', t4);
plot(budgetDesMoines,randomDesMoines,'Color',"green");
hold on
plot(budgetDesMoines,greedyDesMoines,'Color',"cyan");
plot(budgetDesMoines,optimizedDesMoines,'Color',"red");
xlabel('Budget');
ylabel('Coverage');
legend('Random','Greedy','Optimized');
grid on
title(ax4, 'DeMoines');

t5 = uitab(tg);
ax5 = axes('Parent', t5);
plot(budgetLuisVille,randomLuisVille,'Color',"green");
hold on
plot(budgetLuisVille,greedyLuisVille,'Color',"cyan");
plot(budgetLuisVille,optimizedLuisVille,'Color',"red");
xlabel('Budget');
ylabel('Coverage');
legend('Random','Greedy','Optimized');
grid on
title(ax5, 'Luisville');