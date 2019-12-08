using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace HearthServer.Migrations
{
    public partial class AddedNewFields : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Measurements_Sessions_SessionId",
                table: "Measurements");

            migrationBuilder.DropColumn(
                name: "Timestamp",
                table: "Measurements");

            migrationBuilder.AddColumn<int>(
                name: "MillisDelta",
                table: "Measurements",
                nullable: false,
                defaultValue: 0);

            migrationBuilder.AddForeignKey(
                name: "FK_Measurements_Sessions_SessionId",
                table: "Measurements",
                column: "SessionId",
                principalTable: "Sessions",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Measurements_Sessions_SessionId",
                table: "Measurements");

            migrationBuilder.DropColumn(
                name: "MillisDelta",
                table: "Measurements");

            migrationBuilder.AddColumn<DateTime>(
                name: "Timestamp",
                table: "Measurements",
                type: "TEXT",
                nullable: false,
                defaultValue: new DateTime(1, 1, 1, 0, 0, 0, 0, DateTimeKind.Unspecified));

            migrationBuilder.AddForeignKey(
                name: "FK_Measurements_Sessions_SessionId",
                table: "Measurements",
                column: "SessionId",
                principalTable: "Sessions",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }
    }
}
