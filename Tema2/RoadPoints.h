#pragma once

#include "utils/glm_utils.h"
#include <vector>
#include <string>
#include <iostream>

namespace RoadPoints {
    class Points
    {
        public:
            Points(){
                points = { glm::vec3(0,0,0),
                       glm::vec3(5, 0, 5),
                       glm::vec3(7.46, 0, 10.84),
                       glm::vec3(6.64, 0, 18.65),
                       glm::vec3(1.28, 0, 22.47),
                       glm::vec3(-5.43, 0, 23.01),
                       glm::vec3(-13.97, 0, 19.74),
                       glm::vec3(-18.61, 0, 13.75),
                       glm::vec3(-19.61, 0, 7.57),
                       glm::vec3(-30.6, 0, 0.75),
                       glm::vec3(-38.41, 0, 0),
                       glm::vec3(-46.22, 0, 4.17),
                       glm::vec3(-48.88, 0, 13.08),
                       glm::vec3(-47.28, 0, 22.93),
                       glm::vec3(-40, 0, 30),
                       glm::vec3(-31.85, 0, 35.7),
                       glm::vec3(-20.68, 0, 41.68),
                       glm::vec3(-10.7, 0, 44.48),
                       glm::vec3(1.4, 0, 45.54),
                       glm::vec3(10.31, 0, 44.74),
                       glm::vec3(17.76, 0, 40.49),
                       glm::vec3(21.75, 0, 34.63),
                       glm::vec3(26.3, 0, 29.06),
                       glm::vec3(35.06, 0, 27.39),
                       glm::vec3(43.61, 0, 28.01),
                       glm::vec3(51.32, 0, 34.27),
                       glm::vec3(51.95, 0, 45.53),
                       glm::vec3(45.9, 0, 56.79),
                       glm::vec3(36.31, 0, 62.84),
                       glm::vec3(27.76, 0, 64.84),
                       glm::vec3(14.62, 0, 69.93),
                       glm::vec3(3.57, 0, 73.89),
                       glm::vec3(-9.56, 0, 79.73),
                       glm::vec3(-20, 0, 80),
                       glm::vec3(-30, 0, 77.86),
                       glm::vec3(-37.51, 0, 70.56),
                       glm::vec3(-45.22, 0, 66.8),
                       glm::vec3(-55.86, 0, 65.34),
                       glm::vec3(-64.2, 0, 67.64),
                       glm::vec3(-67.54, 0, 75.15),
                       glm::vec3(-65.04, 0, 89.33),
                       glm::vec3(-60, 0, 100),
                       glm::vec3(-56.28, 0, 111.64),
                       glm::vec3(-60, 0, 120),
                       glm::vec3(-69.62, 0, 129.99),
                       glm::vec3(-80, 0, 140),
                       glm::vec3(-82.97, 0, 149.6),
                       glm::vec3(-81.93, 0, 164.2),
                       glm::vec3(-76.3, 0, 177.54),
                       glm::vec3(-66.7, 0, 184.84),
                       glm::vec3(-53.57, 0, 190.06),
                       glm::vec3(-40.22, 0, 190.06),
                       glm::vec3(-28.33, 0, 182.55),
                       glm::vec3(-24.58, 0, 171.7),
                       glm::vec3(-24.37, 0, 160.44),
                       glm::vec3(-26.87, 0, 146.68),
                       glm::vec3(-30, 0, 132.91),
                       glm::vec3(-27.29, 0, 121.44),
                       glm::vec3(-17.49, 0, 112.89),
                       glm::vec3(-6.02, 0, 110.39),
                       glm::vec3(11.49, 0, 113.1),
                       glm::vec3(22.55, 0, 125.61),
                       glm::vec3(22.75, 0, 142.51),
                       glm::vec3(17.75, 0, 154.81),
                       glm::vec3(12.74, 0, 166.49),
                       glm::vec3(11.91, 0, 182.76),
                       glm::vec3(18.17, 0, 197.36),
                       glm::vec3(33.18, 0, 208.2),
                       glm::vec3(51.32, 0, 211.12),
                       glm::vec3(71.97, 0, 204.45),
                       glm::vec3(80.94, 0, 186.93),
                       glm::vec3(79.9, 0, 169.62),
                       glm::vec3(74.68, 0, 153.98),
                       glm::vec3(68.84, 0, 139.59),
                       glm::vec3(63.64, 0, 124.3),
                       glm::vec3(61.26, 0, 103.91),
                       glm::vec3(66.7, 0, 95.08),
                       glm::vec3(77.57, 0, 89.64),
                       glm::vec3(93.2, 0, 85.56),
                       glm::vec3(110.19, 0, 81.49),
                       glm::vec3(127.86, 0, 76.73),
                       glm::vec3(141.45, 0, 67.89),
                       glm::vec3(146.55, 0, 52.94),
                       glm::vec3(146.55, 0, 38.67),
                       glm::vec3(141.45, 0, 25.3),
                       glm::vec3(130.65, 0, 21.22),
                       glm::vec3(120, 0, 20),
                       glm::vec3(104.01, 0, 19.54),
                       glm::vec3(94.33, 0, 14.74),
                       glm::vec3(93.21, 0, 7.07),
                       glm::vec3(94.17, 0, -3.48),
                       glm::vec3(100.17, 0, -10.2),
                       glm::vec3(109.53, 0, -14.04),
                       glm::vec3(129.69, 0, -18.12),
                       glm::vec3(168.11, 0, -23.91),
                       glm::vec3(199.23, 0, -29.09),
                       glm::vec3(220, 0, -40),
                       glm::vec3(234.05, 0, -52.43),
                       glm::vec3(242.58, 0, -70.59),
                       glm::vec3(241.46, 0, -83.55),
                       glm::vec3(234.8, 0, -97.63),
                       glm::vec3(219.24, 0, -108.01),
                       glm::vec3(194.41, 0, -119.86),
                       glm::vec3(171.07, 0, -125.05),
                       glm::vec3(147.36, 0, -127.27),
                       glm::vec3(129.95, 0, -122.08),
                       glm::vec3(117.72, 0, -112.45),
                       glm::vec3(96.97, 0, -92.07),
                       glm::vec3(87.71, 0, -70.96),
                       glm::vec3(77.71, 0, -55.77),
                       glm::vec3(62.15, 0, -42.8),
                       glm::vec3(43.25, 0, -36.87),
                       glm::vec3(29.17, 0, -41.32),
                       glm::vec3(17.32, 0, -56.14),
                       glm::vec3(13.24, 0, -70.96),
                       glm::vec3(12.87, 0, -83.92),
                       glm::vec3(13.98, 0, -98.74),
                       glm::vec3(16.21, 0, -115.79),
                       glm::vec3(20, 0, -140),
                       glm::vec3(30.29, 0, -158.02),
                       glm::vec3(38.26, 0, -175.79),
                       glm::vec3(45.29, 0, -195.46),
                       glm::vec3(45.29, 0, -215.49),
                       glm::vec3(34.68, 0, -237.05),
                       glm::vec3(14.14, 0, -253.64),
                       glm::vec3(-11.92, 0, -259.56),
                       glm::vec3(-39.97, 0, -255.61),
                       glm::vec3(-65.25, 0, -244.95),
                       glm::vec3(-87.37, 0, -224.41),
                       glm::vec3(-100, 0, -200),
                       glm::vec3(-103.96, 0, -171.08),
                       glm::vec3(-111.47, 0, -143.83),
                       glm::vec3(-121.74, 0, -127.63),
                       glm::vec3(-135.96, 0, -115.78),
                       glm::vec3(-154.13, 0, -110.64),
                       glm::vec3(-173.09, 0, -110.64),
                       glm::vec3(-188.1, 0, -127.23),
                       glm::vec3(-194.02, 0, -145.8),
                       glm::vec3(-194.02, 0, -163.18),
                       glm::vec3(-197.18, 0, -195.97),
                       glm::vec3(-214.17, 0, -223.22),
                       glm::vec3(-230.36, 0, -237.44),
                       glm::vec3(-252.88, 0, -242.18),
                       glm::vec3(-277.37, 0, -237.05),
                       glm::vec3(-294.35, 0, -223.62),
                       glm::vec3(-305.81, 0, -198.34),
                       glm::vec3(-311.73, 0, -168.32),
                       glm::vec3(-313.71, 0, -138.69),
                       glm::vec3(-312.92, 0, -109.46),
                       glm::vec3(-310.55, 0, -86.55),
                       glm::vec3(-300.28, 0, -64.03),
                       glm::vec3(-285.66, 0, -51.79),
                       glm::vec3(-265.91, 0, -46.26),
                       glm::vec3(-241.82, 0, -47.05),
                       glm::vec3(-215.75, 0, -54.95),
                       glm::vec3(-194.42, 0, -59.29),
                       glm::vec3(-170.72, 0, -62.06),
                       glm::vec3(-149.78, 0, -60.87),
                       glm::vec3(-129.64, 0, -55.74),
                       glm::vec3(-107.91, 0, -49.81),
                       glm::vec3(-85, 0, -47.05),
                       glm::vec3(-62.48, 0, -47.05),
                       glm::vec3(-46.29, 0, -47.05),
                       glm::vec3(-29.3, 0, -44.68),
                       glm::vec3(-17.85, 0, -32.43),
                       glm::vec3(-10.74, 0, -16.63)
                };

                /* initialize toPass and nextToPass in order to identify start of race */
                toPass = 0;
                nextToPass = points.size() - 1;
            }
            ~Points(){}

            std::vector<glm::vec3> GetPoints() {
                return points;
            }

            bool CheckPosition(glm::vec3 position) {
                if (distance(position, points[toPass]) > distance(position, points[nextToPass])) {
                    /* if car passed the last point of points, return true, meaning a new tour has started */
                    toPass--;
                    nextToPass--;
                    if (toPass == -1) {
                        toPass = points.size() - 1;
                        return true;
                    }
                    if (nextToPass == -1) {
                        nextToPass = points.size() - 1;
                    }
                }
                return false;
            }

        public:
            std::vector<glm::vec3> points;
            int toPass;
            int nextToPass;

    };
   
}