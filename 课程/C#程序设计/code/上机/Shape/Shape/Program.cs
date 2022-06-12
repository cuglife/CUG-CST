using System;

namespace ThreeDimensional
{
    public abstract class Abstract3D
    {
        /**
     * get the perimeter of the graph
     *
     * @return Perimeter of the graph
     */
        public abstract double GetVolume();

        /**
     * get the area of the graph
     *
     * @return area of the graph
     */
        public abstract double GetArea();
    }

    internal interface IColor
    {
        /**
     * set the color of graphic
     *
     * @param color the color of graphic
     */
        void SetColor(String color);
    }

    internal class Sphere : Abstract3D, IColor
    {
        private double r = 0;
        private string color = "null";

        public Sphere(double r)
        {
            this.r = r;
        }

        public override double GetVolume()
        {
            return (4 / 3.0 * Math.PI * Math.Pow(r, 3));
        }

        public override double GetArea()
        {
            return 4 * Math.PI * r * r;
        }

        public void SetColor(string color)
        {
            this.color = color;
        }
    }

    internal class Cylinder : Abstract3D, IColor
    {
        private double r = 0;
        private double h = 0;
        private string color = "";

        Cylinder(double r, double h)
        {
            this.r = r;
            this.h = h;
        }

        public override double GetVolume()
        {
            return Math.PI * r * r * h;
        }

        public override double GetArea()
        {
            return Math.PI * r * r * 2 + 2 * Math.PI * r * h;
        }

        public void SetColor(string color)
        {
            this.color = color;
        }
    }

    internal class Cone : Abstract3D, IColor
    {
        private double r;
        private double h;
        private string color;

        Cone(double r, double h)
        {
            this.r = r;
            this.h = h;
        }

        public override double GetVolume()
        {
            return 1 / 3.0 * Math.PI * r * r * h;
        }

        public override double GetArea()
        {
            return Math.PI * r * (r + Math.Sqrt(r * r + h * h));
        }

        public void SetColor(string color)
        {
            this.color = color;
        }
    }
}