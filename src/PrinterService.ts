import { Injectable } from '@nestjs/common';
import { exec } from 'child_process';
import { promisify } from 'util';

const execPromise = promisify(exec);

@Injectable()
export class PrinterService {
  private printerProxyPath = '/path/to/PrinterProxy';

  private async executeCommand(command: string): Promise<string> {
    try {
      const { stdout, stderr } = await execPromise(command);
      if (stderr) {
        throw new Error(stderr);
      }
      return stdout;
    } catch (error) {
      throw new Error(`Command failed: ${error.message}`);
    }
  }

  /**
   * Setup the printer with the specified parameters.
   * @param printSpeed - The print speed.
   * @param printDensity - The print density.
   * @param labelWidth - The label width.
   * @param labelHeight - The label height.
   * @param labelType - The label type.
   * @param gapHeight - The gap height.
   * @param offset - The offset.
   */
  async setupPrinter(printSpeed: number, printDensity: number, labelWidth: number, labelHeight: number, labelType: number, gapHeight: number, offset: number): Promise<string> {
    const command = `${this.printerProxyPath} setup ${printSpeed} ${printDensity} ${labelWidth} ${labelHeight} ${labelType} ${gapHeight} ${offset}`;
    return this.executeCommand(command);
  }

  /**
   * Clear the printer buffer.
   */
  async clearBuffer(): Promise<string> {
    const command = `${this.printerProxyPath} clearbuffer`;
    return this.executeCommand(command);
  }

  /**
   * Print the specified number of copies.
   * @param num - The number of labels to print.
   * @param copies - The number of copies to print.
   */
  async print(num: number, copies: number): Promise<string> {
    const command = `${this.printerProxyPath} print ${num} ${copies}`;
    return this.executeCommand(command);
  }

  /**
   * Get the printer status.
   */
  async getStatus(): Promise<string> {
    const command = `${this.printerProxyPath} status`;
    return this.executeCommand(command);
  }

  /**
   * Print a sample label.
   */
  async printSample(): Promise<string> {
    const command = `${this.printerProxyPath} sample`;
    return this.executeCommand(command);
  }

  /**
   * Print a QR code with the specified parameters.
   * @param x - The x-coordinate.
   * @param y - The y-coordinate.
   * @param width - The width of the QR code.
   * @param eccLevel - The error correction level.
   * @param mode - The mode.
   * @param rotate - The rotation.
   * @param model - The model.
   * @param mask - The mask.
   * @param data - The data to encode in the QR code.
   */
  async printQRCode(x: number, y: number, width: number, eccLevel: number, mode: number, rotate: number, model: number, mask: number, data: string): Promise<string> {
    const command = `${this.printerProxyPath} qrcode ${x} ${y} ${width} ${eccLevel} ${mode} ${rotate} ${model} ${mask} ${data}`;
    return this.executeCommand(command);
  }

  /**
   * Print a barcode with the specified parameters.
   * @param x - The x-coordinate.
   * @param y - The y-coordinate.
   * @param type - The type of barcode.
   * @param content - The content of the barcode.
   * @param height - The height of the barcode.
   * @param showText - Whether to show text.
   * @param rotation - The rotation.
   * @param narrow - The narrow bar width.
   * @param wide - The wide bar width.
   */
  async printBarCode(x: number, y: number, type: number, content: string, height: number, showText: number, rotation: number, narrow: number, wide: number): Promise<string> {
    const command = `${this.printerProxyPath} barcode ${x} ${y} ${type} ${content} ${height} ${showText} ${rotation} ${narrow} ${wide}`;
    return this.executeCommand(command);
  }

  /**
   * Print text with the specified parameters.
   * @param x - The x-coordinate.
   * @param y - The y-coordinate.
   * @param fontName - The font name.
   * @param content - The content of the text.
   * @param rotation - The rotation.
   * @param x_multiplication - The x multiplication.
   * @param y_multiplication - The y multiplication.
   * @param alignment - The alignment.
   */
  async printText(x: number, y: number, fontName: string, content: string, rotation: number, x_multiplication: number, y_multiplication: number, alignment: number): Promise<string> {
    const command = `${this.printerProxyPath} text ${x} ${y} ${fontName} ${content} ${rotation} ${x_multiplication} ${y_multiplication} ${alignment}`;
    return this.executeCommand(command);
  }

  /**
   * Print an image with the specified parameters.
   * @param x - The x-coordinate.
   * @param y - The y-coordinate.
   * @param mode - The mode.
   * @param path - The path to the image file.
   */
  async printImage(x: number, y: number, mode: number, path: string): Promise<string> {
    const command = `${this.printerProxyPath} image ${x} ${y} ${mode} ${path}`;
    return this.executeCommand(command);
  }

  /**
   * Select the printer by index.
   * @param printerIndex - The index of the printer to select.
   */
  async selectPrinter(printerIndex: number): Promise<string> {
    const command = `${this.printerProxyPath} select ${printerIndex}`;
    return this.executeCommand(command);
  }
}
