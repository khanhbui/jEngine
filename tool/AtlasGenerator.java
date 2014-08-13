import java.util.*;
import java.io.*;
import javax.imageio.*;
import java.awt.Rectangle;
import java.awt.Graphics2D;
import java.awt.image.*;
import java.awt.RenderingHints;

public class AtlasGenerator
{	
	public static void main(String args[])
	{
		if(args.length < 4)
		{
			System.out.println("Texture Atlas Generator by Lukasz Bruun - lukasz.dk");
			System.out.println("\tUsage: AtlasGenerator <name> <width> <height> <padding> <ignorePaths> <unitCoordinates> <directory> [<directory> ...]");
			System.out.println("\t\t<padding>: Padding between images in the final texture atlas.");
			System.out.println("\t\t<ignorePaths>: Only writes out the file name without the path of it to the atlas txt file.");
			System.out.println("\t\t<unitCoordinates>: Coordinates will be written to atlas txt file in 0..1 range instead of 0..width, 0..height range");
			System.out.println("\tExample: AtlasGenerator atlas 2048 2048 5 1 1 images");
			return;
		}

		AtlasGenerator atlasGenerator = new AtlasGenerator();
		List<File> dirs = new ArrayList<File>();
		for(int i = 6; i < args.length; ++i)
		{
			dirs.add(new File(args[i]));
		}
		atlasGenerator.Run(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]), Integer.parseInt(args[3]), Integer.parseInt(args[4]) != 0, Integer.parseInt(args[5]) != 0, dirs);
	}

	public void Run(String name, int width, int height, int padding, boolean fileNameOnly, boolean unitCoordinates, List<File> dirs)
	{
		List<File> imageFiles = new ArrayList<File>();

		for(File file : dirs)
		{
			if(!file.exists() || !file.isDirectory())
			{
				System.out.println("Error: Could not find directory '" + file.getPath() + "'");
				return;
			}

			GetImageFiles(file, imageFiles);
		}

		System.out.println("Found " + imageFiles.size() + " images");

		Set<ImageName> imageNameSet = new TreeSet<ImageName>(new ImageNameComparator());
		
		for(File f : imageFiles)
		{
			try
			{
				BufferedImage image = ImageIO.read(f);
                
                int imgWidth = image.getWidth() * 2;
                int imgHeight = image.getHeight() * 2;
				
				if(imgWidth > width || imgHeight > height)
				{
					System.out.println("Error: '" + f.getPath() + "' (" + imgWidth + "x" + imgHeight + ") is larger than the atlas (" + width + "x" + height + ")");
					return;
				}
				
				String path = f.getPath().substring(0, f.getPath().lastIndexOf(".")).replace("\\", "/");
				
				imageNameSet.add(new ImageName(image, path));
				
			}
			catch(IOException e)
			{
				System.out.println("Could not open file: '" + f.getAbsoluteFile() + "'");
			}
		}
		
		List<Texture> textures = new ArrayList<Texture>();
		
		textures.add(new Texture(width, height));
		
		int count = 0;
		
		for(ImageName imageName : imageNameSet)
		{
			boolean added = false;
			
			System.out.println("Adding " + imageName.name + " to atlas (" + (++count) + ")");
			
			for(Texture texture : textures)
			{
				if(texture.AddImage(imageName.image, imageName.name, padding))
				{
					added = true;
					break;
				}
			}
			
			if(!added)
			{
				Texture texture = new Texture(width, height);
				texture.AddImage(imageName.image, imageName.name, padding);
				textures.add(texture);
			}
		}
		
		count = 0;
		
		for(Texture texture : textures)
		{
			System.out.println("Writing atlas: " + name + (++count));
			texture.Write(name + count, fileNameOnly, unitCoordinates, width, height);
		}
	}
	
	private void GetImageFiles(File file, List<File> imageFiles)
	{
		if(file.isDirectory())
		{
			File[] files = file.listFiles(new ImageFilenameFilter());
			File[] directories = file.listFiles(new DirectoryFileFilter());
		
			imageFiles.addAll(Arrays.asList(files));
			
			for(File d : directories)
			{
				GetImageFiles(d, imageFiles);
			}
		}
	}
	
	private class ImageName 
	{
		public BufferedImage image;
		public String name;
		
		public ImageName(BufferedImage image, String name)
		{
			this.image = image;
			this.name = name;
		}
	}
	
	private class ImageNameComparator implements Comparator<ImageName>
	{
		public int compare(ImageName image1, ImageName image2)
		{
			int area1 = image1.image.getWidth() * image1.image.getHeight() * 4;
			int area2 = image2.image.getWidth() * image2.image.getHeight() * 4;
		
			if(area1 != area2)
			{
				return area2 - area1;
			}
			else
			{
				return image1.name.compareTo(image2.name);
			}
		}
	}
		
	private class ImageFilenameFilter implements FilenameFilter
	{
		public boolean accept(File dir, String name)
		{
			return name.toLowerCase().endsWith(".png");
		}
	}

	private class DirectoryFileFilter implements FileFilter
	{
		public boolean accept(File pathname) 
		{
			return pathname.isDirectory();
		}
	}
	
	public class Texture
	{
		private class Node
		{
			public Rectangle rect;
			public Node child[];
			public BufferedImage image;
			
			public Node(int x, int y, int width, int height)
			{
				rect = new Rectangle(x, y, width, height);
				child = new Node[2];
				child[0] = null;
				child[1] = null;
				image = null;
			}
			
			public boolean IsLeaf()
			{
				return child[0] == null && child[1] == null;
			}
			
			// Algorithm from http://www.blackpawn.com/texts/lightmaps/
			public Node Insert(BufferedImage image, int padding)
			{
                int imgWidth = image.getWidth() * 2;
                int imgHeight = image.getHeight() * 2;
                
				if(!IsLeaf())
				{
					Node newNode = child[0].Insert(image, padding);
					
					if(newNode != null)
					{
						return newNode;
					}
					
					return child[1].Insert(image, padding);
				}
				else
				{
					if(this.image != null) 
					{
						return null; // occupied
					}
					
					if(imgWidth > rect.width || imgHeight > rect.height)
					{
						return null; // does not fit
					}
										
					if(imgWidth == rect.width && imgHeight == rect.height) 
					{						
						this.image = image; // perfect fit
						return this;
					}
					
					int dw = rect.width - imgWidth;
					int dh = rect.height - imgHeight;
					
					if(dw > dh)
					{
						child[0] = new Node(rect.x, rect.y, imgWidth, rect.height);
						child[1] = new Node(padding + rect.x + imgWidth, rect.y, rect.width - imgWidth - padding, rect.height);
					}
					else
					{
						child[0] = new Node(rect.x, rect.y, rect.width, imgHeight);
						child[1] = new Node(rect.x, padding + rect.y + imgHeight, rect.width, rect.height - imgHeight - padding);
					}
					/*if(dw > dh)
					{
						child[0] = new Node(rect.x, rect.y, image.getWidth(), rect.height);
						child[1] = new Node(padding+rect.x+image.getWidth(), rect.y, rect.width - image.getWidth(), rect.height);
					}
					else
					{
						child[0] = new Node(rect.x, rect.y, rect.width, image.getHeight());
						child[1] = new Node(rect.x, padding+rect.y+image.getHeight(), rect.width, rect.height - image.getHeight());
					}*/
					
					return child[0].Insert(image, padding);
				}
			}
		}
		
		private BufferedImage image;
		private Graphics2D graphics;
		private Node root;
		private Map<String, Rectangle> rectangleMap;

		public Texture(int width, int height)
		{
			image = new BufferedImage(width, height, BufferedImage.TYPE_4BYTE_ABGR);
			graphics = image.createGraphics();
			
			root = new Node(0,0, width, height);
			rectangleMap = new TreeMap<String, Rectangle>();
		}
		
		public boolean AddImage(BufferedImage image, String name, int padding)
		{
			Node node = root.Insert(image, padding);
			
			if(node == null)
			{
				return false;				
			}
			
			rectangleMap.put(name, node.rect);
			//graphics.drawImage(image, null, node.rect.x, node.rect.y);
            graphics.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
            graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
            graphics.drawImage(image, node.rect.x, node.rect.y, node.rect.x + image.getWidth() * 2, node.rect.y + image.getHeight() * 2, 0, 0, image.getWidth(), image.getHeight(), null);
			
			
			return true;	
		}
		
		public void Write(String name, boolean fileNameOnly, boolean unitCoordinates, int width, int height)
		{			
			try
			{
				ImageIO.write(image, "png", new File(name + ".png"));
				
				BufferedWriter atlas = new BufferedWriter(new FileWriter(name + ".txt"));
				
				for(Map.Entry<String, Rectangle> e : rectangleMap.entrySet())
				{
					Rectangle r = e.getValue();
					String keyVal = e.getKey();
					if (fileNameOnly)
						keyVal = keyVal.substring(keyVal.lastIndexOf('/') + 1);
					if (unitCoordinates)
					{
						atlas.write(keyVal + " " + r.width + " " + r.height + " " + r.x/(float)width + " " + r.y/(float)height + " " + r.width/(float)width + " " + r.height/(float)height);
					}
					else
						atlas.write(keyVal + " " + r.width + " " + r.height + " " + r.x + " " + r.y + " " + r.width + " " + r.height);
					atlas.newLine();
				}
				
				atlas.close();
			}
			catch(IOException e)
			{
				
			}
		}
	}
}
