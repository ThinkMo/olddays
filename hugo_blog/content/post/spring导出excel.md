+++
disqusIdentifier = "2dsada123spring_excel"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2016-11-23T18:47:34+08:00"
showDate = true
clearReading = true
keywords = ["springmvc"]
title = "springmvc实现导出数据excel"
categories = [ "java" ]
+++

springmvc实现导出数据excel

最近在项目中要实现将数据导出为excel的功能，研究了下目前springmvc框架下excel导出的方式，在spring 4.3中使用AbstractXlsView来实现，AbstractExcelView已被弃用；但这里介绍的是直接用apache poi实现的一种方式。

- model如下

```
package red.itech.blog.dao.model;

import java.util.Date;

/**
 * Created by you on 16/10/28.
 */

	public class Blog {
    private String author;
    private Date createdAt;
    private String title;
    private long count;

    public Blog(){}

    public Blog(String author, Date createdAt, String title, long count){
        this.author = author;
        this.count = count;
        this.createdAt = createdAt;
        this.title =title;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public void setCreatedAt(Date createdAt) {
        this.createdAt = createdAt;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setCount(long count) {
        this.count = count;
    }

    public Date getCreatedAt() {
        return createdAt;
    }

    public long getCount() {
        return count;
    }

    public String getAuthor() {
        return author;
    }

    public String getTitle() {
        return title;
    }
}
```
- controller实现

```
package red.itech.blog.controller;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import red.itech.blog.dao.model.Blog;

import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by you on 16/10/28.
 */
@Controller
public class ExportExcel {
    @RequestMapping(value = "/exportExcel", method = RequestMethod.GET)
    public void exportExcel(HttpServletRequest request, HttpServletResponse response){
        //根据业务需求获得数据
        List<Blog> blogs = getBlog();

        Workbook workbook = new HSSFWorkbook();
        Sheet sheet = workbook.createSheet("博客列表");
        Row row = sheet.createRow(0);
        Cell cell = null;
        String[] metaData = new String[]{"作者", "创建日前", "文章题目", "字数"};
        for(int i = 0; i < metaData.length; i++) {
            cell = row.createCell(i);
            cell.setCellType(Cell.CELL_TYPE_STRING);
            cell.setCellValue(metaData[i]);
        }

        int rowNumber = 1;
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:ss:mm");
        for(Blog blog: blogs) {
            row = sheet.createRow(rowNumber++);
            cell = row.createCell(0);
            cell.setCellType(Cell.CELL_TYPE_STRING);
            cell.setCellValue(blog.getAuthor());

            cell = row.createCell(1);
            cell.setCellType(Cell.CELL_TYPE_STRING);
            cell.setCellValue(df.format(blog.getCreatedAt()));

            cell = row.createCell(2);
            cell.setCellType(Cell.CELL_TYPE_STRING);
            cell.setCellValue(blog.getAuthor());

            cell = row.createCell(3);
            cell.setCellType(Cell.CELL_TYPE_NUMERIC);
            cell.setCellValue(blog.getCount());
        }
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        try {
            workbook.write(os);
        }catch (Exception e){
            e.printStackTrace();
        }
        byte[] content = os.toByteArray();
        InputStream is = new ByteArrayInputStream(content);
        // 设置response参数，可以打开下载页面
        response.reset();
        response.setContentType("application/vnd.ms-excel;charset=utf-8");
        try {
            response.setHeader("Content-Disposition", "attachment;filename=" + new String(("blogs.xls").getBytes(), "iso-8859-1"));
            ServletOutputStream out = response.getOutputStream();
            BufferedInputStream bis = null;
            BufferedOutputStream bos = null;
            try {
                bis = new BufferedInputStream(is);
                bos = new BufferedOutputStream(out);
                byte[] buff = new byte[2048];
                int bytesRead;
                // Simple read/write loop.
                while (-1 != (bytesRead = bis.read(buff, 0, buff.length))) {
                    bos.write(buff, 0, bytesRead);
                }
            } catch (final IOException e) {
                throw e;
            } finally {
                if (bis != null)
                    bis.close();
                if (bos != null)
                    bos.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private List<Blog> getBlog(){
        List<Blog> blogs = new ArrayList<Blog>();
        blogs.add(new Blog("Tom", new Date(), "how to work?", 1024));
        blogs.add(new Blog("Jim", new Date(), "Let it be", 677));
        blogs.add(new Blog("Jerry", new Date(), "Make a cake", 3024));
        blogs.add(new Blog("Kate", new Date(), "To be a good engineer", 5624));
        return blogs;
    }
}
```

- jsp页面

```
<%--
  Created by IntelliJ IDEA.
  User: you
  Date: 16/10/28
  Time: 上午10:42
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
  <head>
  </head>
  <body>
  <input type="button" value="导出数据" onclick="download()"/>
  </body>
<script>
  function download(){
    var url = '/exportExcel';
    window.open(url);
  }
</script>
</html>
```









